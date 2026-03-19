--[[
  Author: Daniel Dehaan
  Website: www.danielrdehaan.com
  Description: This script processes selected media items in REAPER, 
               attempting to open a project file specified in the 
               "BWF:Description" metadata of each item's source file. 
               If the description starts with "RPP:", it treats the 
               following text as a project file path and attempts to 
               open it in a new project tab.
]]

-- Get the number of selected media items
local num_selected_items = reaper.CountSelectedMediaItems(0)
if num_selected_items == 0 then
  reaper.ShowMessageBox("No media items selected.", "Error", 0)
  return
end

-- Function to open project in new tab
function OpenProjectInNewTab(project_path)
  -- Create a new project tab
  reaper.Main_OnCommand(40859, 0) -- "New project tab (ignore default template)"

  -- We need to wait a moment to ensure the new tab is fully initialized
  reaper.defer(function()
    -- Open the project in the new tab without prompting to save
    reaper.Main_openProject('noprompt:' .. project_path)
  end)
end

-- Loop through selected media items (in case multiple are selected)
for i = 0, num_selected_items - 1 do
  -- Get the media item
  local item = reaper.GetSelectedMediaItem(0, i)
  if item then
    -- Get the active take of the media item
    local take = reaper.GetActiveTake(item)
    if take and not reaper.TakeIsMIDI(take) then
      -- Get the source of the take
      local source = reaper.GetMediaItemTake_Source(take)
      if source then
        -- Debug Step: Get and display the source file path
        local source_filename = reaper.GetMediaSourceFileName(source, "")
        -- reaper.ShowConsoleMsg("Source file path: " .. source_filename .. "\n")

        -- Try to retrieve the "BWF:Description" metadata field
        local ret, desc = reaper.GetMediaFileMetadata(source, "BWF:Description", "", false)
        if ret and desc ~= "" then
          -- reaper.ShowConsoleMsg("Description metadata: " .. desc .. "\n")
          -- Check if the description starts with "RPP:"
          if desc:sub(1, 4) == "RPP:" then
            -- Extract the project file path
            local project_path = desc:sub(5)
            -- reaper.ShowConsoleMsg("Project file path found: " .. project_path .. "\n")
            -- Check if the project file exists
            if reaper.file_exists(project_path) then
              OpenProjectInNewTab(project_path)
            else
              reaper.ShowMessageBox("Project file does not exist:\n" .. project_path, "Error", 0)
            end
          else
            reaper.ShowMessageBox("No project file path found in media item's metadata.", "Error", 0)
          end
        else
          reaper.ShowMessageBox("Failed to retrieve media item's metadata.", "Error", 0)
        end
      else
        reaper.ShowMessageBox("Failed to get source of the active take for media item.", "Error", 0)
      end
    else
      reaper.ShowMessageBox("No active audio take found for the media item.", "Error", 0)
    end
  else
    reaper.ShowMessageBox("Failed to get media item.\n")
  end
end
