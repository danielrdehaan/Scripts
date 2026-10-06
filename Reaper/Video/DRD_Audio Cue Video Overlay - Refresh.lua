--[[
  Author: Daniel Dehaan
  Website: www.danielrdehaan.com
  Description: Rebuilds the Audio Cue Video Overlay from the settings saved in
               the project, without showing the dialog. Bind it to a shortcut
               and run it before previewing or rendering. Opens the dialog
               only if the overlay hasn't been set up in this project yet.

  Requirements:
    - "DRD_Audio Cue Video Overlay.lua" in the same folder
]]

local dir = debug.getinfo(1, "S").source:match("@?(.*[\\/])")
ACVO_MODE = "refresh"
dofile(dir .. "DRD_Audio Cue Video Overlay.lua")
