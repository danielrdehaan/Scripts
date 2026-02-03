// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Common declarations for ptslcmd application.
 */

#pragma once

#include "PTSLC_CPP/CppPTSLClient.h"
#include "PTSLC_CPP/CppPTSLCommonConversions.h"

#include "CommandLineParser.h"
#include "PtslCmdCommandResult.h"

using namespace std;
using namespace PTSLC_CPP;

#define MAP_ENTRY(C, T)                                                                                                \
    {                                                                                                                  \
        C::T, #T                                                                                                       \
    }
#define RVRS_MAP_ENTRY(C, T)                                                                                           \
    {                                                                                                                  \
        #T, C::T                                                                                                       \
    }
#define DEF_CMD(CMD) PtslCmdCommandResult CMD(const vector<string>& params, CppPTSLClient& client)

DEF_CMD(CreateSession);
DEF_CMD(OpenSession);
DEF_CMD(Import);
DEF_CMD(GetTrackList);
DEF_CMD(SelectAllClipsOnTrack);
DEF_CMD(ExtendSelectionToTargetTracks);
DEF_CMD(TrimToSelection);
DEF_CMD(CreateFadesBasedOnPreset);
DEF_CMD(RenameTargetTrack);
DEF_CMD(ConsolidateClip);
DEF_CMD(ExportClipsAsFiles);
DEF_CMD(ExportSelectedTracksAsAAFOMF);
DEF_CMD(GetTaskStatus);
DEF_CMD(RefreshTargetAudioFiles);
DEF_CMD(RefreshAllModifiedAudioFiles);
DEF_CMD(GetFileLocation);
DEF_CMD(CloseSession);
DEF_CMD(SaveSession);
DEF_CMD(SaveSessionAs);
DEF_CMD(Cut);
DEF_CMD(Copy);
DEF_CMD(Paste);
DEF_CMD(Clear);
DEF_CMD(CutSpecial);
DEF_CMD(CopySpecial);
DEF_CMD(ClearSpecial);
DEF_CMD(PasteSpecial);
DEF_CMD(ExportMix);
DEF_CMD(Spot);
DEF_CMD(ExportSessionInfoAsText);
DEF_CMD(SetPlaybackMode);
DEF_CMD(SetRecordMode);
DEF_CMD(GetDynamicProperties);
DEF_CMD(GetSessionAudioFormat);
DEF_CMD(GetSessionSampleRate);
DEF_CMD(GetSessionBitDepth);
DEF_CMD(GetSessionInterleavedState);
DEF_CMD(GetSessionTimeCodeRate);
DEF_CMD(GetSessionFeetFramesRate);
DEF_CMD(GetSessionAudioRatePullSettings);
DEF_CMD(GetSessionVideoRatePullSettings);
DEF_CMD(GetSessionName);
DEF_CMD(GetSessionPath);
DEF_CMD(GetSessionStartTime);
DEF_CMD(GetSessionLength);
DEF_CMD(SetSessionAudioFormat);
DEF_CMD(SetSessionBitDepth);
DEF_CMD(SetSessionInterleavedState);
DEF_CMD(SetSessionTimeCodeRate);
DEF_CMD(SetSessionFeetFramesRate);
DEF_CMD(SetSessionAudioRatePullSettings);
DEF_CMD(SetSessionVideoRatePullSettings);
DEF_CMD(SetSessionStartTime);
DEF_CMD(SetSessionLength);
DEF_CMD(GetPTSLVersion);
DEF_CMD(GetPlaybackMode);
DEF_CMD(GetRecordMode);
DEF_CMD(GetTransportArmed);
DEF_CMD(GetTransportState);
DEF_CMD(ClearMemoryLocation);
DEF_CMD(RenameSelectedClip);
DEF_CMD(RenameTargetClip);
DEF_CMD(TogglePlayState);
DEF_CMD(ToggleRecordEnable);
DEF_CMD(PlayHalfSpeed);
DEF_CMD(RecordHalfSpeed);
DEF_CMD(EditMemoryLocation);
DEF_CMD(CreateMemoryLocation);
DEF_CMD(GetMemoryLocations);
DEF_CMD(RegisterConnection);
DEF_CMD(CreateNewTracks);
DEF_CMD(GetEditMode);
DEF_CMD(SetEditMode);
DEF_CMD(GetEditTool);
DEF_CMD(SetEditTool);
DEF_CMD(RecallZoomPreset);
DEF_CMD(GetEditModeOptions);
DEF_CMD(SetEditModeOptions);
DEF_CMD(SetTimelineSelection);
DEF_CMD(GetTimelineSelection);
DEF_CMD(SelectTracksByName);
DEF_CMD(ImportVideo);
DEF_CMD(SelectMemoryLocation);
DEF_CMD(SetTrackMuteState);
DEF_CMD(SetTrackSoloState);
DEF_CMD(SetTrackSoloSafeState);
DEF_CMD(SetTrackRecordEnableState);
DEF_CMD(SetTrackRecordSafeEnableState);
DEF_CMD(SetTrackInputMonitorState);
DEF_CMD(SetTrackSmartDspState);
DEF_CMD(SetTrackHiddenState);
DEF_CMD(SetTrackInactiveState);
DEF_CMD(SetTrackFrozenState);
DEF_CMD(SetTrackOnlineState);
DEF_CMD(SetTrackOpenState);
DEF_CMD(GetSessionIDs);
DEF_CMD(GetMemoryLocationsManageMode);
DEF_CMD(SetMemoryLocationsManageMode);
DEF_CMD(SetMainCounterFormat);
DEF_CMD(SetSubCounterFormat);
DEF_CMD(GetMainCounterFormat);
DEF_CMD(GetSubCounterFormat);
DEF_CMD(Undo);
DEF_CMD(Redo);
DEF_CMD(UndoAll);
DEF_CMD(RedoAll);
DEF_CMD(ClearUndoQueue);
DEF_CMD(SetTrackDSPModeSafeState);
DEF_CMD(GetSessionSystemDelayInfo);
DEF_CMD(GroupClips);
DEF_CMD(UngroupClips);
DEF_CMD(UngroupAllClips);
DEF_CMD(RegroupClips);
DEF_CMD(HostReadyCheck);
DEF_CMD(RepeatSelection);
DEF_CMD(DuplicateSelection);
DEF_CMD(ClearAllMemoryLocations);

// This is a temporary func until we start using JSON request as the only available request input. Then refactoring is needed.
inline bool FillCommandRequestByDirectJson(const std::vector<std::string>& params, CommandRequest& request)
{
    // TODO: (ashevche, 24.01.2024) We need to extract a common header file which would contain these constants:
    const std::string jsonRequestArgName = "-json_request"; // whether the request is in a JSON form;
                                                            // soon, it'll become the only option
                                                            // (once we get rid of CLI command args parsing)

    const std::string isJsonRequestDirectArgName = "-is_json_request_direct"; // whether the JSON request should be
                                                                              // passed directly to gRPC client

    for (size_t i = 0; i < params.size(); ++i)
    {
        if (params[i] == jsonRequestArgName && i + 1 < params.size())
        {
            request.directJsonBody = params[i + 1];

            bool isJsonRequestDirect = true; // currently, if the request is in a JSON form, we always pass it directly
                                             // until we start parsing JSON similarly to cmd arguments as we do now;
                                             // then, we'll need to take a look at -is_json_request_direct arg
            return isJsonRequestDirect;
        }
    }

    // once -json_request becomes the only option, we should return an error here
    return false;
}
