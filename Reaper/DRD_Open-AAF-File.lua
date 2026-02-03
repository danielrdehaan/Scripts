--[[
  Author: Daniel Dehaan
  Website: www.danielrdehaan.com
  Description: This script opens a file dialog allowing the user to select
               an AAF (Advanced Authoring Format) file and imports it into
               Reaper using the reaper-aaf Python script.

  Requirements:
    - Python 3 with pyaaf2 installed (pip3 install pyaaf2)
    - DRD_Import-AAF.py in the same Scripts folder
]]

-- Get the script's directory
local info = debug.getinfo(1, "S")
local script_path = info.source:match("@?(.*[\\/])")
local python_script = script_path .. "DRD_Import-AAF.py"

-- Debug function to print to console
local function Debug(msg)
  reaper.ShowConsoleMsg("[AAF Import] " .. msg .. "\n")
end

-- Function to get the file extension
local function GetFileExtension(path)
  return path:match("^.+%.(.+)$")
end

-- Function to check if file is an AAF
local function IsAAFFile(path)
  local ext = GetFileExtension(path)
  return ext and ext:lower() == "aaf"
end

-- Function to check if a file exists
local function FileExists(path)
  local f = io.open(path, "r")
  if f then
    f:close()
    return true
  end
  return false
end

-- Function to escape shell arguments
local function ShellEscape(arg)
  return "'" .. arg:gsub("'", "'\\''") .. "'"
end

-- Function to find Python 3
local function FindPython()
  local python_paths = {
    "/Library/Frameworks/Python.framework/Versions/3.9/bin/python3",
    "/Library/Frameworks/Python.framework/Versions/3.10/bin/python3",
    "/Library/Frameworks/Python.framework/Versions/3.11/bin/python3",
    "/Library/Frameworks/Python.framework/Versions/3.12/bin/python3",
    "/opt/homebrew/bin/python3",
    "/usr/local/bin/python3",
    "/usr/bin/python3"
  }

  for _, path in ipairs(python_paths) do
    if FileExists(path) then
      return path
    end
  end

  return nil
end

-- Simple JSON parser for the specific format we expect
local function ParseJSON(str)
  -- Remove whitespace
  str = str:gsub("^%s+", ""):gsub("%s+$", "")

  local pos = 1
  local function skip_whitespace()
    while pos <= #str and str:sub(pos, pos):match("%s") do
      pos = pos + 1
    end
  end

  local function parse_string()
    if str:sub(pos, pos) ~= '"' then return nil end
    pos = pos + 1
    local start = pos
    while pos <= #str do
      local c = str:sub(pos, pos)
      if c == '"' then
        local result = str:sub(start, pos - 1)
        pos = pos + 1
        -- Handle escape sequences
        result = result:gsub('\\"', '"')
        result = result:gsub('\\\\', '\\')
        result = result:gsub('\\n', '\n')
        result = result:gsub('\\t', '\t')
        return result
      elseif c == '\\' then
        pos = pos + 2
      else
        pos = pos + 1
      end
    end
    return nil
  end

  local function parse_number()
    local start = pos
    if str:sub(pos, pos) == '-' then pos = pos + 1 end
    while pos <= #str and str:sub(pos, pos):match("[0-9]") do pos = pos + 1 end
    if pos <= #str and str:sub(pos, pos) == '.' then
      pos = pos + 1
      while pos <= #str and str:sub(pos, pos):match("[0-9]") do pos = pos + 1 end
    end
    if pos <= #str and str:sub(pos, pos):lower() == 'e' then
      pos = pos + 1
      if str:sub(pos, pos):match("[+-]") then pos = pos + 1 end
      while pos <= #str and str:sub(pos, pos):match("[0-9]") do pos = pos + 1 end
    end
    return tonumber(str:sub(start, pos - 1))
  end

  local parse_value

  local function parse_array()
    if str:sub(pos, pos) ~= '[' then return nil end
    pos = pos + 1
    local arr = {}
    skip_whitespace()
    if str:sub(pos, pos) == ']' then
      pos = pos + 1
      return arr
    end
    while true do
      skip_whitespace()
      local val = parse_value()
      table.insert(arr, val)
      skip_whitespace()
      if str:sub(pos, pos) == ']' then
        pos = pos + 1
        return arr
      elseif str:sub(pos, pos) == ',' then
        pos = pos + 1
      else
        return nil
      end
    end
  end

  local function parse_object()
    if str:sub(pos, pos) ~= '{' then return nil end
    pos = pos + 1
    local obj = {}
    skip_whitespace()
    if str:sub(pos, pos) == '}' then
      pos = pos + 1
      return obj
    end
    while true do
      skip_whitespace()
      local key = parse_string()
      if not key then return nil end
      skip_whitespace()
      if str:sub(pos, pos) ~= ':' then return nil end
      pos = pos + 1
      skip_whitespace()
      local val = parse_value()
      obj[key] = val
      skip_whitespace()
      if str:sub(pos, pos) == '}' then
        pos = pos + 1
        return obj
      elseif str:sub(pos, pos) == ',' then
        pos = pos + 1
      else
        return nil
      end
    end
  end

  parse_value = function()
    skip_whitespace()
    local c = str:sub(pos, pos)
    if c == '"' then
      return parse_string()
    elseif c == '{' then
      return parse_object()
    elseif c == '[' then
      return parse_array()
    elseif c == 't' then
      pos = pos + 4
      return true
    elseif c == 'f' then
      pos = pos + 5
      return false
    elseif c == 'n' then
      pos = pos + 4
      return nil
    else
      return parse_number()
    end
  end

  return parse_value()
end

-- Fix macOS paths that have volume name prepended
local function FixMacPath(path)
  if not path then return path end
  -- Remove "/Macintosh HD" or similar volume prefixes
  local fixed = path:gsub("^/[^/]+ HD/", "/")
  -- Also handle "/Volumes/Macintosh HD/" style paths
  fixed = fixed:gsub("^/Volumes/[^/]+/", "/")
  return fixed
end

-- Build Reaper project from parsed JSON data
function BuildProjectFromJSON(json_str, output_dir)
  Debug("Parsing JSON data...")
  Debug("JSON length: " .. #json_str .. " characters")
  Debug("JSON preview (first 500 chars): " .. json_str:sub(1, 500))

  local data = ParseJSON(json_str)

  if not data then
    Debug("ERROR: Failed to parse JSON")
    Debug("JSON content saved to: " .. output_dir .. "/aaf_debug.json")
    local debug_file = io.open(output_dir .. "/aaf_debug.json", "w")
    if debug_file then
      debug_file:write(json_str)
      debug_file:close()
    end
    reaper.ShowMessageBox("Failed to parse AAF data.\nCheck console for details.", "Error", 0)
    return
  end

  if not data.tracks then
    Debug("ERROR: No tracks in data")
    reaper.ShowMessageBox("No tracks found in AAF file.", "Error", 0)
    return
  end

  Debug("Found " .. #data.tracks .. " tracks")

  reaper.Undo_BeginBlock()
  reaper.PreventUIRefresh(1)

  -- Create tracks and items
  for i, track_data in ipairs(data.tracks) do
    -- Create track
    local track_idx = reaper.CountTracks(0)
    reaper.InsertTrackAtIndex(track_idx, false)
    local track = reaper.GetTrack(0, track_idx)

    -- Set track name
    local track_name = track_data.name or ("Track " .. i)
    reaper.GetSetMediaTrackInfo_String(track, "P_NAME", track_name, true)
    Debug("Created track: " .. track_name)

    -- Set track volume if specified
    if track_data.volume then
      reaper.SetMediaTrackInfo_Value(track, "D_VOL", track_data.volume)
    end

    -- Set track panning if specified
    if track_data.panning then
      reaper.SetMediaTrackInfo_Value(track, "D_PAN", track_data.panning)
    end

    -- Create items
    local items_created = 0
    local items_failed = 0
    if track_data.items then
      for _, item_data in ipairs(track_data.items) do
        if item_data.source and item_data.source ~= "" then
          -- Fix the source path for macOS
          local source_path = FixMacPath(item_data.source)

          -- Create media item
          local item = reaper.AddMediaItemToTrack(track)
          reaper.SetMediaItemInfo_Value(item, "D_POSITION", item_data.position or 0)
          reaper.SetMediaItemInfo_Value(item, "D_LENGTH", item_data.duration or 1)

          -- Add take with source
          local take = reaper.AddTakeToMediaItem(item)
          local source = reaper.PCM_Source_CreateFromFile(source_path)
          if source then
            reaper.SetMediaItemTake_Source(take, source)
            items_created = items_created + 1

            -- Set source offset
            if item_data.offset then
              reaper.SetMediaItemTakeInfo_Value(take, "D_STARTOFFS", item_data.offset)
            end
          else
            items_failed = items_failed + 1
            if items_failed <= 3 then
              Debug("WARNING: Could not load source: " .. source_path)
              Debug("  Original path: " .. item_data.source)
            elseif items_failed == 4 then
              Debug("WARNING: Suppressing further source warnings...")
            end
          end

          -- Set fades
          if item_data.fadein then
            reaper.SetMediaItemInfo_Value(item, "D_FADEINLEN", item_data.fadein)
            reaper.SetMediaItemInfo_Value(item, "C_FADEINSHAPE", item_data.fadeintype or 0)
          end
          if item_data.fadeout then
            reaper.SetMediaItemInfo_Value(item, "D_FADEOUTLEN", item_data.fadeout)
            reaper.SetMediaItemInfo_Value(item, "C_FADEOUTSHAPE", item_data.fadeouttype or 0)
          end

          -- Set item volume
          if item_data.volume then
            reaper.SetMediaItemInfo_Value(item, "D_VOL", item_data.volume)
          end
        end
      end
      Debug("  Items created: " .. items_created .. ", failed: " .. items_failed)
    end

    -- Add volume envelope if specified
    if track_data.volume_envelope and #track_data.volume_envelope > 0 then
      local env = reaper.GetTrackEnvelopeByName(track, "Volume")
      if not env then
        reaper.SetOnlyTrackSelected(track)
        reaper.Main_OnCommand(40406, 0) -- Show volume envelope
        env = reaper.GetTrackEnvelopeByName(track, "Volume")
      end
      if env then
        for _, point in ipairs(track_data.volume_envelope) do
          local scaled = reaper.ScaleToEnvelopeMode(1, point.value)
          reaper.InsertEnvelopePoint(env, point.time, scaled, 0, 0, false, true)
        end
        reaper.Envelope_SortPoints(env)
      end
    end

    -- Add pan envelope if specified
    if track_data.panning_envelope and #track_data.panning_envelope > 0 then
      local env = reaper.GetTrackEnvelopeByName(track, "Pan")
      if not env then
        reaper.SetOnlyTrackSelected(track)
        reaper.Main_OnCommand(40407, 0) -- Show pan envelope
        env = reaper.GetTrackEnvelopeByName(track, "Pan")
      end
      if env then
        for _, point in ipairs(track_data.panning_envelope) do
          reaper.InsertEnvelopePoint(env, point.time, point.value, 0, 0, false, true)
        end
        reaper.Envelope_SortPoints(env)
      end
    end
  end

  -- Create markers
  if data.markers then
    for _, marker in ipairs(data.markers) do
      local color = 0
      if marker.colour then
        color = reaper.ColorToNative(marker.colour.r, marker.colour.g, marker.colour.b) | 0x1000000
      end
      reaper.AddProjectMarker2(0, false, marker.position, 0, marker.name or "", -1, color)
    end
    Debug("Created " .. #data.markers .. " markers")
  end

  reaper.PreventUIRefresh(-1)
  reaper.UpdateArrange()
  reaper.Undo_EndBlock("Import AAF", -1)

  Debug("Project built successfully!")
  reaper.ShowMessageBox(
    "AAF imported successfully!\n\n" ..
    "Tracks: " .. #data.tracks .. "\n" ..
    "Markers: " .. (data.markers and #data.markers or 0),
    "Import Complete", 0
  )
end

-- Main script execution
local function Main()
  Debug("=== AAF Import Script Started ===")

  -- Check for Python
  local python_path = FindPython()
  if not python_path then
    Debug("ERROR: Python 3 not found")
    reaper.ShowMessageBox(
      "Python 3 not found. Please install Python 3 and pyaaf2.\n\n" ..
      "Install from: https://www.python.org/downloads/\n" ..
      "Then run: pip3 install pyaaf2",
      "Python Required", 0
    )
    return
  end
  Debug("Found Python at: " .. python_path)

  -- Check for Python import script
  if not FileExists(python_script) then
    Debug("ERROR: Python script not found at: " .. python_script)
    reaper.ShowMessageBox(
      "DRD_Import-AAF.py not found.\n\n" ..
      "Expected location:\n" .. python_script,
      "Script Missing", 0
    )
    return
  end
  Debug("Found import script at: " .. python_script)

  -- Open file dialog for AAF selection
  local retval, filename = reaper.GetUserFileNameForRead("", "Select AAF File", "aaf")
  Debug("File dialog returned: retval=" .. tostring(retval) .. ", filename=" .. tostring(filename))

  if not retval or filename == "" then
    Debug("User cancelled or no file selected")
    return
  end

  -- Verify the file exists
  if not reaper.file_exists(filename) then
    Debug("ERROR: File does not exist: " .. filename)
    reaper.ShowMessageBox("File does not exist:\n" .. filename, "Error", 0)
    return
  end

  -- Verify it's an AAF file
  if not IsAAFFile(filename) then
    Debug("ERROR: Not an AAF file: " .. filename)
    reaper.ShowMessageBox("Selected file is not an AAF file:\n" .. filename, "Error", 0)
    return
  end

  Debug("Importing AAF: " .. filename)

  -- Get project directory for extracted audio
  local proj_path = reaper.GetProjectPath("")
  if proj_path == "" then
    -- No project saved yet, use AAF directory
    proj_path = filename:match("(.*/)")
  end
  Debug("Project/output directory: " .. proj_path)

  -- Build the command
  local cmd = ShellEscape(python_path) .. " " ..
              ShellEscape(python_script) .. " " ..
              ShellEscape(filename) .. " " ..
              ShellEscape(proj_path)

  Debug("Running command: " .. cmd)

  -- Run the Python script and capture output
  local handle = io.popen(cmd .. " 2>&1")
  if handle then
    local result = handle:read("*a")
    local success, _, code = handle:close()

    Debug("Python output:\n" .. result)

    if result:find('"tracks"') then
      -- Script produced JSON output, now we need to parse and build
      Debug("Received track data from Python script")

      -- Extract just the JSON part (skip any log messages before it)
      local json_start = result:find("{")
      if json_start then
        local json_data = result:sub(json_start)
        Debug("Extracted JSON starting at position: " .. json_start)

        -- Save JSON to temp file
        local json_path = proj_path .. "/aaf_import_temp.json"
        local json_file = io.open(json_path, "w")
        if json_file then
          json_file:write(json_data)
          json_file:close()
          Debug("Saved JSON to: " .. json_path)
        end

        -- Parse JSON and build project
        BuildProjectFromJSON(json_data, proj_path)

        -- Clean up temp file
        os.remove(json_path)
      else
        Debug("ERROR: Could not find JSON start in output")
      end
    else
      Debug("No valid output from Python script")
      reaper.ShowMessageBox(
        "AAF import failed.\n\nPython output:\n" .. result:sub(1, 500),
        "Import Error", 0
      )
    end
  else
    Debug("ERROR: Failed to run Python script")
    reaper.ShowMessageBox("Failed to run Python script.", "Error", 0)
  end

  Debug("=== AAF Import Script Finished ===")
end

-- Run the main function
Main()
