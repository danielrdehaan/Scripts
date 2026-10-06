--[[
  Author: Daniel Dehaan
  Website: www.danielrdehaan.com
  Description: Background toggle that keeps the Audio Cue Video Overlay in
               sync with the timeline. After any edit, once things have been
               quiet for a moment, it rescans the cue items and rewrites the
               overlay only if the cue list actually changed.

  Notes:
    - Only acts on projects where the overlay has already been set up with
      "DRD_Audio Cue Video Overlay.lua"; it never creates the overlay itself.
    - To switch it off, run the action again. The first time, REAPER asks what
      to do with the running instance: choose "Terminate instances" and tick
      "Remember my answer".
    - Run "DRD_Audio Cue Video Overlay - Refresh.lua" before rendering if this
      toggle is off.

  Requirements:
    - "DRD_Audio Cue Video Overlay.lua" in the same folder
]]

local IDLE_DELAY = 0.5 -- seconds without edits before rescanning

local dir = debug.getinfo(1, "S").source:match("@?(.*[\\/])")
ACVO_MODE = "library"
local acvo = dofile(dir .. "DRD_Audio Cue Video Overlay.lua")

local _, _, section, cmd = reaper.get_action_context()
reaper.SetToggleCommandState(section, cmd, 1)
reaper.RefreshToolbar2(section, cmd)
reaper.atexit(function()
  reaper.SetToggleCommandState(section, cmd, 0)
  reaper.RefreshToolbar2(section, cmd)
end)

local reported_error = false

local function sync()
  local track = acvo.find_overlay_track()
  if not track then return end
  local settings = acvo.load_settings()
  if not settings then return end

  local cues = acvo.collect_cues(settings, track)
  if acvo.cue_hash(settings, cues) == acvo.current_hash(track) then return end

  local status = acvo.rebuild(settings, { create = false, undo = false })
  if status == "error" and not reported_error then
    reported_error = true
    reaper.ShowConsoleMsg(acvo.SCRIPT_NAME .. ": couldn't write the Video Processor code (REAPER 7+ required).\n")
  end
end

local last_proj, last_count, dirty_since

local function loop()
  local proj = reaper.EnumProjects(-1)
  local count = reaper.GetProjectStateChangeCount(proj)
  if proj ~= last_proj or count ~= last_count then
    last_proj, last_count = proj, count
    dirty_since = reaper.time_precise()
  end

  if dirty_since and reaper.time_precise() - dirty_since >= IDLE_DELAY then
    dirty_since = nil
    sync()
    -- Our own rewrite bumps the change count; absorb it so it doesn't retrigger
    last_count = reaper.GetProjectStateChangeCount(proj)
  end

  reaper.defer(loop)
end

loop()
