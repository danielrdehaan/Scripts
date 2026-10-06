--[[
  Author: Daniel Dehaan
  Website: www.danielrdehaan.com
  Description: Toggles a crop on video items so only the image area is shown,
               letting the picture fill more of REAPER's video window (works
               best with a wide video window, e.g. docked along the bottom).

  How it works:
    Adds a "Video Crop" Video Processor as take FX on the selected video
    items (or every video item in the project if none are selected) and
    toggles it on/off. The cropped frame becomes the output size, so the
    video window shows just the image.

    If the project has an Audio Cue Overlay ("Cue Overlay" track), the crop
    copies its Mask top/bottom/left/right values and the overlay and timeline
    are switched off while cropped (they draw in the area being cropped away),
    then back on when the crop is removed.

    Crop amounts can be fine-tuned on the "Video Crop" take FX knobs.

  IMPORTANT: Toggle the crop off before rendering, or the render is cropped.
]]

local SCRIPT_NAME = "Toggle Video Crop"
local CROP_FX_NAME = "Video Crop"
local OVERLAY_TRACK_NAME = "Cue Overlay"

-- { variable, knob name, default, matching Audio Cue Overlay knob }
local CROP_PARAMS = {
  { "ct", "Crop top",    0.42, "Mask top" },
  { "cb", "Crop bottom", 0.42, "Mask bottom" },
  { "cl", "Crop left",   0,    "Mask left" },
  { "cr", "Crop right",  0,    "Mask right" },
}

local function crop_code()
  local lines = { "// " .. CROP_FX_NAME }
  for i, p in ipairs(CROP_PARAMS) do
    lines[#lines + 1] = string.format("//@param%d:%s '%s' %s 0 1 0.5 0.001", i, p[1], p[2], p[3])
  end
  lines[#lines + 1] = [==[
input = 0;
input_info(input, iw, ih) ? (
  sx = (cl * iw) | 0; sy = (ct * ih) | 0;
  sw = max(iw - sx - ((cr * iw) | 0), 1);
  sh = max(ih - sy - ((cb * ih) | 0), 1);
  project_w = sw; project_h = sh;
  gfx_blit(input, 0, 0, 0, sw, sh, sx, sy, sw, sh);
);]==]
  return table.concat(lines, "\n")
end

---------------------------------------------------------------------------
-- Helpers
---------------------------------------------------------------------------

local function is_video_take(take)
  if not take then return false end
  local source = reaper.GetMediaItemTake_Source(take)
  -- Reversed/section items wrap the real source
  while source and reaper.GetMediaSourceType(source, "") == "SECTION" do
    source = reaper.GetMediaSourceParent(source)
  end
  return source ~= nil and reaper.GetMediaSourceType(source, "") == "VIDEO"
end

local function target_takes()
  local takes = {}
  for i = 0, reaper.CountSelectedMediaItems(0) - 1 do
    local take = reaper.GetActiveTake(reaper.GetSelectedMediaItem(0, i))
    if is_video_take(take) then takes[#takes + 1] = take end
  end
  if #takes > 0 then return takes end
  for i = 0, reaper.CountMediaItems(0) - 1 do
    local take = reaper.GetActiveTake(reaper.GetMediaItem(0, i))
    if is_video_take(take) then takes[#takes + 1] = take end
  end
  return takes
end

local function find_crop_fx(take)
  for fx = 0, reaper.TakeFX_GetCount(take) - 1 do
    local ok, code = reaper.TakeFX_GetNamedConfigParm(take, fx, "VIDEO_CODE")
    if ok and code:sub(1, #CROP_FX_NAME + 3) == "// " .. CROP_FX_NAME then return fx end
  end
  return nil
end

-- The Cue Overlay track and its Video Processors (overlay + timeline)
local function find_overlay_fx()
  for t = 0, reaper.CountTracks(0) - 1 do
    local track = reaper.GetTrack(0, t)
    local _, name = reaper.GetTrackName(track)
    if name == OVERLAY_TRACK_NAME then
      local fxs = {}
      for fx = 0, reaper.TrackFX_GetCount(track) - 1 do
        if reaper.TrackFX_GetNamedConfigParm(track, fx, "VIDEO_CODE") then fxs[#fxs + 1] = fx end
      end
      if #fxs > 0 then return track, fxs end
    end
  end
  return nil, {}
end

-- Crop amounts keyed by knob name, taken from the overlay's mask if present
local function overlay_mask_values(track, fxs)
  local values = {}
  local wanted = {}
  for _, p in ipairs(CROP_PARAMS) do wanted[p[4]] = p[2] end
  for _, fx in ipairs(fxs) do
    for i = 0, reaper.TrackFX_GetNumParams(track, fx) - 1 do
      local _, name = reaper.TrackFX_GetParamName(track, fx, i)
      if wanted[name] then values[wanted[name]] = reaper.TrackFX_GetParam(track, fx, i) end
    end
  end
  return values
end

local function set_take_params(take, fx, values)
  for i = 0, reaper.TakeFX_GetNumParams(take, fx) - 1 do
    local _, name = reaper.TakeFX_GetParamName(take, fx, i)
    if values[name] ~= nil then reaper.TakeFX_SetParam(take, fx, i, values[name]) end
  end
end

---------------------------------------------------------------------------
-- Main
---------------------------------------------------------------------------

local function main()
  local takes = target_takes()
  if #takes == 0 then
    reaper.MB("No video items found. Select the video item(s) to crop.", SCRIPT_NAME, 0)
    return
  end

  -- Crop if any target is currently uncropped, otherwise uncrop all
  local crop = false
  for _, take in ipairs(takes) do
    local fx = find_crop_fx(take)
    if not fx or not reaper.TakeFX_GetEnabled(take, fx) then crop = true break end
  end

  local overlay_track, overlay_fxs = find_overlay_fx()
  local mask = overlay_mask_values(overlay_track, overlay_fxs)

  reaper.Undo_BeginBlock()
  reaper.PreventUIRefresh(1)

  local failed = false
  for _, take in ipairs(takes) do
    local fx = find_crop_fx(take)
    if crop then
      if not fx then
        fx = reaper.TakeFX_AddByName(take, "Video processor", -1)
        if fx < 0 or not reaper.TakeFX_SetNamedConfigParm(take, fx, "VIDEO_CODE", crop_code()) then
          failed = true
        else
          reaper.TakeFX_SetNamedConfigParm(take, fx, "renamed_name", CROP_FX_NAME)
          local defaults = {}
          for _, p in ipairs(CROP_PARAMS) do defaults[p[2]] = p[3] end
          set_take_params(take, fx, defaults)
        end
      end
      if not failed then
        set_take_params(take, fx, mask)
        reaper.TakeFX_SetEnabled(take, fx, true)
      end
    elseif fx then
      reaper.TakeFX_SetEnabled(take, fx, false)
    end
  end

  for _, fx in ipairs(overlay_fxs) do
    reaper.TrackFX_SetEnabled(overlay_track, fx, not crop)
  end

  reaper.PreventUIRefresh(-1)
  reaper.UpdateArrange()
  reaper.Undo_EndBlock(crop and "Crop video" or "Uncrop video", -1)

  local _, _, section, cmd = reaper.get_action_context()
  reaper.SetToggleCommandState(section, cmd, crop and 1 or 0)
  reaper.RefreshToolbar2(section, cmd)

  if failed then
    reaper.MB("Couldn't add the Video Crop processor. This script needs REAPER 7 or newer.", SCRIPT_NAME, 0)
  end
end

main()
