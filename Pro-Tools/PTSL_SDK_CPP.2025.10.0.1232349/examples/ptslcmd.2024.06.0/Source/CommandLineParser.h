// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define DEF_CMD_STRINGS(CMD)                                                                                           \
    extern const std::string g_psz##CMD;                                                                               \
    extern const std::string g_psz##CMD##Help;

DEF_CMD_STRINGS(CreateSession)
DEF_CMD_STRINGS(OpenSession)
DEF_CMD_STRINGS(Import)
DEF_CMD_STRINGS(GetTrackList)
DEF_CMD_STRINGS(SelectAllClipsOnTrack)
DEF_CMD_STRINGS(ExtendSelectionToTargetTracks)
DEF_CMD_STRINGS(TrimToSelection)
DEF_CMD_STRINGS(CreateFadesBasedOnPreset)
DEF_CMD_STRINGS(RenameTargetTrack)
DEF_CMD_STRINGS(ConsolidateClip)
DEF_CMD_STRINGS(ExportClipsAsFiles)
DEF_CMD_STRINGS(ExportSelectedTracksAsAAFOMF)
DEF_CMD_STRINGS(GetTaskStatus)
DEF_CMD_STRINGS(RefreshTargetAudioFiles)
DEF_CMD_STRINGS(RefreshAllModifiedAudioFiles)
DEF_CMD_STRINGS(GetFileLocation)
DEF_CMD_STRINGS(CloseSession)
DEF_CMD_STRINGS(SaveSession)
DEF_CMD_STRINGS(SaveSessionAs)
DEF_CMD_STRINGS(Cut)
DEF_CMD_STRINGS(Copy)
DEF_CMD_STRINGS(Paste)
DEF_CMD_STRINGS(Clear)
DEF_CMD_STRINGS(CutSpecial)
DEF_CMD_STRINGS(CopySpecial)
DEF_CMD_STRINGS(ClearSpecial)
DEF_CMD_STRINGS(PasteSpecial)
DEF_CMD_STRINGS(ExportMix)
DEF_CMD_STRINGS(Spot)
DEF_CMD_STRINGS(ExportSessionInfoAsText)
DEF_CMD_STRINGS(SetPlaybackMode)
DEF_CMD_STRINGS(SetRecordMode)
DEF_CMD_STRINGS(GetDynamicProperties)
DEF_CMD_STRINGS(GetSessionAudioFormat)
DEF_CMD_STRINGS(GetSessionSampleRate)
DEF_CMD_STRINGS(GetSessionBitDepth)
DEF_CMD_STRINGS(GetSessionInterleavedState)
DEF_CMD_STRINGS(GetSessionTimeCodeRate)
DEF_CMD_STRINGS(GetSessionFeetFramesRate)
DEF_CMD_STRINGS(GetSessionAudioRatePullSettings)
DEF_CMD_STRINGS(GetSessionVideoRatePullSettings)
DEF_CMD_STRINGS(GetSessionName)
DEF_CMD_STRINGS(GetSessionPath)
DEF_CMD_STRINGS(GetSessionStartTime)
DEF_CMD_STRINGS(GetSessionLength)
DEF_CMD_STRINGS(SetSessionAudioFormat)
DEF_CMD_STRINGS(SetSessionBitDepth)
DEF_CMD_STRINGS(SetSessionInterleavedState)
DEF_CMD_STRINGS(SetSessionTimeCodeRate)
DEF_CMD_STRINGS(SetSessionFeetFramesRate)
DEF_CMD_STRINGS(SetSessionAudioRatePullSettings)
DEF_CMD_STRINGS(SetSessionVideoRatePullSettings)
DEF_CMD_STRINGS(SetSessionStartTime)
DEF_CMD_STRINGS(SetSessionLength)
DEF_CMD_STRINGS(GetPTSLVersion)
DEF_CMD_STRINGS(GetPlaybackMode)
DEF_CMD_STRINGS(GetRecordMode)
DEF_CMD_STRINGS(GetTransportArmed)
DEF_CMD_STRINGS(GetTransportState)
DEF_CMD_STRINGS(ClearMemoryLocation)
DEF_CMD_STRINGS(RenameSelectedClip)
DEF_CMD_STRINGS(RenameTargetClip)
DEF_CMD_STRINGS(TogglePlayState)
DEF_CMD_STRINGS(ToggleRecordEnable)
DEF_CMD_STRINGS(PlayHalfSpeed)
DEF_CMD_STRINGS(RecordHalfSpeed)
DEF_CMD_STRINGS(EditMemoryLocation)
DEF_CMD_STRINGS(CreateMemoryLocation)
DEF_CMD_STRINGS(GetMemoryLocations)
DEF_CMD_STRINGS(RegisterConnection)
DEF_CMD_STRINGS(CreateNewTracks)
DEF_CMD_STRINGS(GetEditMode)
DEF_CMD_STRINGS(SetEditMode)
DEF_CMD_STRINGS(GetEditTool)
DEF_CMD_STRINGS(SetEditTool)
DEF_CMD_STRINGS(RecallZoomPreset)
DEF_CMD_STRINGS(GetEditModeOptions)
DEF_CMD_STRINGS(SetEditModeOptions)
DEF_CMD_STRINGS(SetTimelineSelection)
DEF_CMD_STRINGS(GetTimelineSelection)
DEF_CMD_STRINGS(SelectTracksByName)
DEF_CMD_STRINGS(ImportVideo)
DEF_CMD_STRINGS(SelectMemoryLocation)
DEF_CMD_STRINGS(SetTrackMuteState)
DEF_CMD_STRINGS(SetTrackSoloState)
DEF_CMD_STRINGS(SetTrackSoloSafeState)
DEF_CMD_STRINGS(SetTrackRecordEnableState)
DEF_CMD_STRINGS(SetTrackRecordSafeEnableState)
DEF_CMD_STRINGS(SetTrackInputMonitorState)
DEF_CMD_STRINGS(SetTrackSmartDspState)
DEF_CMD_STRINGS(SetTrackHiddenState)
DEF_CMD_STRINGS(SetTrackInactiveState)
DEF_CMD_STRINGS(SetTrackFrozenState)
DEF_CMD_STRINGS(SetTrackOnlineState)
DEF_CMD_STRINGS(SetTrackOpenState)
DEF_CMD_STRINGS(GetSessionIDs)
DEF_CMD_STRINGS(GetMemoryLocationsManageMode)
DEF_CMD_STRINGS(SetMemoryLocationsManageMode)
DEF_CMD_STRINGS(SetMainCounterFormat)
DEF_CMD_STRINGS(SetSubCounterFormat)
DEF_CMD_STRINGS(GetMainCounterFormat)
DEF_CMD_STRINGS(GetSubCounterFormat)
DEF_CMD_STRINGS(Undo)
DEF_CMD_STRINGS(Redo)
DEF_CMD_STRINGS(UndoAll)
DEF_CMD_STRINGS(RedoAll)
DEF_CMD_STRINGS(ClearUndoQueue)
DEF_CMD_STRINGS(SetTrackDSPModeSafeState)
DEF_CMD_STRINGS(GetSessionSystemDelayInfo)
DEF_CMD_STRINGS(GroupClips)
DEF_CMD_STRINGS(UngroupClips)
DEF_CMD_STRINGS(UngroupAllClips)
DEF_CMD_STRINGS(RegroupClips)
DEF_CMD_STRINGS(HostReadyCheck)
DEF_CMD_STRINGS(RepeatSelection)
DEF_CMD_STRINGS(DuplicateSelection)
DEF_CMD_STRINGS(ClearAllMemoryLocations)

class CommandLineParser
{
public:
    struct PtslcmdModifiers
    {
        bool isHelp;
        bool isNamesListOnly;
        bool useFileScript;
        bool useJsonRequest;
        bool useJsonOutput;
    };

    static const std::string PARAMETERLESS_ARGS_KEY;
    static const std::string PARAMETER_MISSED;
    static const std::string PARAMETER_WRONG_USAGE;
    static const std::string PARAMETERLESS_ARGS;
    static const std::string INVALID_COMMAND;
    static const std::string NO_RESPONSE_RECEIVED;

public:
    static PtslcmdModifiers RetrievePtslcmdModifiers(
        const std::vector<std::string>& cmdArgs, bool containsExePath = true);
    static bool HasPtslCommand(const std::vector<std::string>& cmdArgs, size_t ptslCommandPos = 1);

    /// <summary> Retrieves command parameters (e.g., -supported_platforms) with theirs arguments (e.g. x64 x86). </summary>
    /// <param name="count">Number of parameters used by the command</param>
    /// <param name="params">Command line parameters</param>
    /// <param name="paramSymbol">Special symbol for identifying parameters, <code>'-'</code> by default.</param>
    /// <returns> A map, whose key is parameter's name, and the value is a vector of the parameter's arguments. Bare arguments added without any parameter will be returned under the special key <code>"_"</code>.</returns>
    static std::map<std::string, std::vector<std::string>> RetrieveParamsWithArgs(
        const std::vector<std::string>& params, const char paramSymbol = '-');

    static bool IsNumber(const std::string& str);
    static bool StringToBool(const std::string& str);

    static void PrintListCommands(bool namesOnly = false, bool toFormat = true);
    static void PrintHelp(const std::string& command, bool toFormat = true);

private:
    static const std::map<std::string, std::string>& GetCmdUsages();

    static void DoPrintFormattedUsage(const std::string& usage);
    static void DoPrintCommandHelp(const std::string& command, bool toFormat = true, bool nameOnly = false);
};
