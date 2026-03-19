--[[
   Randomize Active Take on Item Start (No consecutive items choose the same take)
   By: Daniel Dehaan
   Description:
   This script runs in the background. When playback starts playing into an item,
   if the item has multiple takes, it will randomly set one as the active take,
   ensuring that it does not select the same take as the last triggered item.
--]]

-- Keep track of which items we were "inside" the last time we checked
local lastInsideState = {} -- keyed by item GUID, value is a boolean

-- Keep track of the last chosen take across all items
local lastChosenTake = nil

-- Persistent state for toggling
local scriptID = "RandomizeActiveTakeOnItemStart"
local isRunning = reaper.GetExtState(scriptID, "Running") == "1"

-- Function to randomly select a take index from 0 to takeCount-1,
-- excluding a specific index (if provided)
local function getRandomTakeExcluding(takeCount, excludeIndex)
    if takeCount == 1 then
        -- Only one take, no choice
        return 0
    else
        local chosen
        repeat
            chosen = math.random(0, takeCount - 1)
        until chosen ~= excludeIndex
        return chosen
    end
end

-- Function to randomize take for a given item, ensuring no repeat of the last chosen take
local function randomizeItemTake(item)
    local takeCount = reaper.CountTakes(item)
    if takeCount > 1 then
        local randomTake = getRandomTakeExcluding(takeCount, lastChosenTake)
        -- Apply chosen take
        reaper.SetMediaItemTakeInfo_Value(reaper.GetMediaItemTake(item, randomTake), "B_DONE", 0) -- force update
        reaper.SetActiveTake(reaper.GetMediaItemTake(item, randomTake))
        reaper.UpdateItemInProject(item)
        -- Remember this choice for the next item
        lastChosenTake = randomTake
    elseif takeCount == 1 then
        -- Only one take. Just set it (though it doesn't add variety)
        reaper.SetActiveTake(reaper.GetMediaItemTake(item, 0))
        lastChosenTake = 0
    end
end

-- Main function that will run every defer cycle
local function main()
    if not isRunning then return end -- Stop if script is no longer running

    if reaper.GetPlayState() & 1 == 1 then
        -- Playback is running
        local playPos = reaper.GetPlayPosition()
        local numItems = reaper.CountMediaItems(0)

        for i = 0, numItems - 1 do
            local item = reaper.GetMediaItem(0, i)
            local itemStart = reaper.GetMediaItemInfo_Value(item, "D_POSITION")
            local itemLength = reaper.GetMediaItemInfo_Value(item, "D_LENGTH")
            local itemEnd = itemStart + itemLength
            local itemGUID = reaper.BR_GetMediaItemGUID(item)

            -- Check if we're currently "inside" this item
            local currentlyInside = (playPos >= itemStart and playPos < itemEnd)
            local wasInside = lastInsideState[itemGUID] or false

            if currentlyInside and not wasInside then
                -- We have just entered the item
                randomizeItemTake(item)
            end

            -- Update state
            lastInsideState[itemGUID] = currentlyInside
        end
    else
        -- Playback not running, reset inside states so that next time we start playing into items,
        -- they will be recognized as "just entered".
        lastInsideState = {}
    end

    reaper.defer(main) -- keep running
end

-- Start the script
local function start()
    isRunning = true
    reaper.SetExtState(scriptID, "Running", "1", false)
    reaper.SetToggleCommandState(0, reaper.NamedCommandLookup("_RS3c8906fd21461481fab94273779975007ec92e15"), 1)
    --reaper.RefreshToolbar2(0)
    main()
end

-- Stop the script
local function stop()
    isRunning = false
    reaper.SetExtState(scriptID, "Running", "0", false)
    reaper.SetToggleCommandState(0, reaper.NamedCommandLookup("_RS3c8906fd21461481fab94273779975007ec92e15"), 0)
    --reaper.RefreshToolbar2(0)
end

-- Toggle script state
if isRunning then
    stop()
    --reaper.ShowConsoleMsg("Randomize Active Take: Disabled\n")
else
    math.randomseed(os.time())
    start()
    --reaper.ShowConsoleMsg("Randomize Active Take: Enabled\n")
end
