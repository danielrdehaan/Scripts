// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Provides the current versions of the PTSL SDK and its API methods (commands).
 * 
 * @details
 * 
 * PTSL SDK versions are defined both for the PTSL connection API as a whole and for each PTSL Client API command.
 * 
 * Compatibility information for specific releases is noted in the 'Release Notes' tab.
 * 
 * The versions are managed by the PTSL SDK developers, so C++ Client Wrapper customers don't have to worry about them.
 * 
 * The API version used by the C++ Client Wrapper is passed to Pro Tools as a default field of @ref ptsl::RequestHeader
 * (see CppPTSLC_DefaultRequest.cpp)
 * 
 * If there is a change in the connection API, or a change in the C++ PTSL Client wrapper implementation which breaks
 * backwards compatibility, the value of @ref PTSL_VERSION will be updated.
 * 
 * If an application that uses the C++ Client Wrapper attempts to issue PTSL commands to a Pro Tools application that
 * does not support the client's API version, a @ref PTSLC_CPP::CommandErrorType::SDK_VersionMismatch "SDK_VersionMismatch"
 * error will be returned with an appropriate error message.
 * 
 * As soon as an API method (command) of the PTSL Client wrapper is changed, its version will be updated in
 * @ref PTSL_CLIENT_API_VERSIONS . Currently these command versions are not used.
 */

#pragma once

#include <map>
#include <string>

/**
* The version of PTSL Host SDK or PTSL Client wrapper itself, depending on where the PTSL_Versions.h file is included.
*/
static const int32_t PTSL_VERSION = 2025;

/*
* Alias for PTSL_VERSION constant for consistency with other version constants.
*
* Supported starting in Pro Tools 2024.10
*/
static const int32_t PTSL_VERSION_MAJOR = PTSL_VERSION;

/**
* The minor version of PTSL Host SDK or PTSL Client wrapper itself, depending on where the PTSL_Versions.h file is included.
* Should be updated manually when PT Host, PTSL Server or PTSL Client wrapper updates and goes to release.
*/
static const int32_t PTSL_VERSION_MINOR = 10;

/**
* The revision version of PTSL Host SDK or PTSL Client wrapper itself, depending on where the PTSL_Versions.h file is included.
* Should be updated manually when PT Host, PTSL Server or PTSL Client wrapper updates and goes to release.
*/
static const int32_t PTSL_VERSION_REVISION = 0;

/**
* The versions of PTSL Client API methods.
*/
static const std::map<std::string, int32_t> PTSL_CLIENT_API_VERSIONS = {
    { "CreateSession", 1 },
    { "OpenSession", 1 },
    { "Import", 1 },
    { "GetTrackList", 1 },
    { "SelectAllClipsOnTrack", 1 },
    { "ExtendSelectionToTargetTracks", 1 },
    { "TrimToSelection", 1 },
    { "CreateFadesBasedOnPreset", 1 },
    { "RenameTargetTrack", 1 },
    { "ConsolidateClip", 1 },
    { "ExportClipsAsFiles", 1 },
    { "ExportSelectedTracksAsAAFOMF", 1 },
    { "GetTaskStatus", 1 },
    { "HostReadyCheck", 2 },
    { "RefreshTargetAudioFiles", 1 },
    { "RefreshAllModifiedAudioFiles", 1 },
    { "GetFileLocation", 1 },
    { "CloseSession", 1 },
    { "SaveSession", 1 },
    { "SaveSessionAs", 1 },
    { "Cut", 1 },
    { "Copy", 1 },
    { "Paste", 1 },
    { "Clear", 1 },
    { "CutSpecial", 1 },
    { "CopySpecial", 1 },
    { "ClearSpecial", 1 },
    { "PasteSpecial", 1 },
    { "ExportMix", 1 },
    { "Spot", 1 },
    { "ExportSessionInfoAsText", 1 },
    { "GetDynamicProperties", 1 },
    { "SetPlaybackMode", 1 },
    { "SetRecordMode", 1 },
    { "GetSessionAudioFormat", 1 },
    { "GetSessionSampleRate", 1 },
    { "GetSessionBitDepth", 1 },
    { "GetSessionInterleavedState", 1 },
    { "GetSessionTimeCodeRate", 1 },
    { "GetSessionFeetFramesRate", 1 },
    { "GetSessionAudioRatePullSettings", 1 },
    { "GetSessionVideoRatePullSettings", 1 },
    { "GetSessionName", 1 },
    { "GetSessionPath", 1 },
    { "GetSessionStartTime", 1 },
    { "GetSessionLength", 1 },
    { "SetSessionAudioFormat", 1 },
    { "SetSessionBitDepth", 1 },
    { "SetSessionInterleavedState", 1 },
    { "SetSessionTimeCodeRate", 1 },
    { "SetSessionFeetFramesRate", 1 },
    { "SetSessionAudioRatePullSettings", 1 },
    { "SetSessionVideoRatePullSettings", 1 },
    { "SetSessionStartTime", 1 },
    { "SetSessionLength", 1 },
    { "GetPTSLVersion", 1 },
    { "GetPlaybackMode", 1 },
    { "GetRecordMode", 1 },
    { "GetTransportArmed", 1 },
    { "GetTransportState", 1 },
    { "ClearMemoryLocation", 1 },
    { "RenameSelectedClip", 1 },
    { "RenameTargetClip", 1 },
    { "TogglePlayState", 1 },
    { "ToggleRecordEnable", 1 },
    { "PlayHalfSpeed", 1 },
    { "RecordHalfSpeed", 1 },
    { "EditMemoryLocation", 1 },
    { "CreateMemoryLocation", 1 },
    { "GetMemoryLocations", 1 },
    { "RegisterConnection", 1 },
    { "CreateNewTracks", 1 },
    { "GetEditMode", 1 },
    { "SetEditMode", 1 },
    { "GetEditTool", 1 },
    { "SetEditTool", 1 },
    { "RecallZoomPreset", 1 },
    { "GetEditModeOptions", 1 },
    { "SetEditModeOptions", 1 },
    { "SetTimelineSelection", 1 },
    { "GetTimelineSelection", 1 },
    { "SelectTracksByName", 1 },
    { "ImportVideo", 1 },
    { "SelectMemoryLocation", 1 },
    { "SetTrackMuteState", 1 },
    { "SetTrackSoloState", 1 },
    { "SetTrackSoloSafeState", 1 },
    { "SetTrackRecordEnableState", 1 },
    { "SetTrackRecordSafeEnableState", 1 },
    { "SetTrackInputMonitorState", 1 },
    { "SetTrackSmartDspState", 1 },
    { "SetTrackHiddenState", 1 },
    { "SetTrackInactiveState", 1 },
    { "SetTrackFrozenState", 1 },
    { "SetTrackOnlineState", 1 },
    { "SetTrackOpenState", 1 },
    { "GetSessionIDs", 1 },
    { "GetMemoryLocationsManageMode", 1 },
    { "SetMemoryLocationsManageMode", 1 },
    { "SetMainCounterFormat", 1 },
    { "SetSubCounterFormat", 1 },
    { "GetMainCounterFormat", 1 },
    { "GetSubCounterFormat", 1 },
    { "Undo", 1 },
    { "Redo", 1 },
    { "UndoAll", 1 },
    { "RedoAll", 1 },
    { "ClearUndoQueue", 1 },
    { "SetTrackDSPModeSafeState", 1 },
    { "GetSessionSystemDelayInfo", 1 },
    { "GroupClips", 1 },
    { "UngroupClips", 1 },
    { "UngroupAllClips", 1 },
    { "RegroupClips", 1 },
    { "RepeatSelection", 1 },
    { "DuplicateSelection", 1 },
    { "ClearAllMemoryLocations", 1 },
};
