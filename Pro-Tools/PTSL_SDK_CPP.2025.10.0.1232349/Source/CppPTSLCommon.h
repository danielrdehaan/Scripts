// Copyright 2020-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Declarations of values and types that are used by the PTSLC_CPP::CppPTSLClient.
 * 
 * See PTSLC_CPP::ClientConfig to view configuration data for PTSLC_CPP::CppPTSLClient::CppPTSLClient initialization.<br>
 * For available commands see PTSLC_CPP::CommandId. Each command should have associated method in the PTSLC_CPP::CppPTSLClient.<br>
 * Each command method takes request data that is either PTSLC_CPP::CommandRequest or a class that inherits it.<br>
 * Each command method returns response data that is either PTSLC_CPP::CommandResponse or a class that inherits it.
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace PTSLC_CPP
{
    enum class Mode : int32_t
    {
        /** Locate Pro Tools application server */
        ProTools [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::Mode::Mode_ProTools instead
        Mode_ProTools = 0,

        /** Locate server directly from client

            Use this if you want to test a client without Pro Tools
        */
        Standalone [[deprecated]] =
            1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::Mode::Mode_Standalone instead
        Mode_Standalone = 1
    };

    enum class SkipHostLaunch : int32_t
    {
        /** Skip auto launch */
        Yes [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::SkipHostLaunch::SHLaunch_Yes instead
        SHLaunch_Yes = 0,

        /** Do not skip auto launch, try to launch Pro Tools if it is not running already */
        No [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::SkipHostLaunch::SHLaunch_No instead
        SHLaunch_No = 1
    };

    /**
     * Structure that describes data needed for client's configuring.
     */
    struct ClientConfig
    {
        std::string address;
        Mode serverMode;
        SkipHostLaunch skipHostLaunch;
    };

    /**
     * Pro Tools command type.
     * 
     * For documentation of each command, see @ref CppPTSLClient
     */
    enum class CommandId : int32_t
    {
        CId_None = -1,

        CreateSession [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CreateSession instead
        CId_CreateSession = 0,

        OpenSession [[deprecated]] =
            1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_OpenSession instead
        CId_OpenSession = 1,

        Import [[deprecated]] =
            2, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Import instead
        CId_Import = 2,

        GetTrackList [[deprecated]] =
            3, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetTrackList instead
        CId_GetTrackList = 3,

        SelectAllClipsOnTrack [[deprecated]] =
            4, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SelectAllClipsOnTrack instead
        CId_SelectAllClipsOnTrack = 4,

        ExtendSelectionToTargetTracks [[deprecated]] =
            5, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ExtendSelectionToTargetTracks instead
        CId_ExtendSelectionToTargetTracks = 5,

        TrimToSelection [[deprecated]] =
            6, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_TrimToSelection instead
        CId_TrimToSelection = 6,

        CreateFadesBasedOnPreset [[deprecated]] =
            7, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CreateFadesBasedOnPreset instead
        CId_CreateFadesBasedOnPreset = 7,

        RenameTargetTrack [[deprecated]] =
            8, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RenameTargetTrack instead
        CId_RenameTargetTrack = 8,

        ConsolidateClip [[deprecated]] =
            9, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ConsolidateClip instead
        CId_ConsolidateClip = 9,

        ExportClipsAsFiles [[deprecated]] =
            10, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ExportClipsAsFiles instead
        CId_ExportClipsAsFiles = 10,

        ExportSelectedTracksAsAAFOMF [[deprecated]] =
            11, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ExportSelectedTracksAsAAFOMF instead
        CId_ExportSelectedTracksAsAAFOMF = 11,

        GetTaskStatus [[deprecated]] =
            12, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetTaskStatus instead
        CId_GetTaskStatus = 12,

        HostReadyCheck [[deprecated]] =
            13, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_HostReadyCheck instead
        CId_HostReadyCheck = 13,

        RefreshTargetAudioFiles [[deprecated]] =
            14, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RefreshTargetAudioFiles instead
        CId_RefreshTargetAudioFiles = 14,

        RefreshAllModifiedAudioFiles [[deprecated]] =
            15, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RefreshAllModifiedAudioFiles instead
        CId_RefreshAllModifiedAudioFiles = 15,

        GetFileLocation [[deprecated]] =
            16, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetFileLocation instead
        CId_GetFileLocation = 16,

        CloseSession [[deprecated]] =
            17, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CloseSession instead
        CId_CloseSession = 17,

        SaveSession [[deprecated]] =
            18, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SaveSession instead
        CId_SaveSession = 18,

        SaveSessionAs [[deprecated]] =
            19, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SaveSessionAs instead
        CId_SaveSessionAs = 19,

        Cut [[deprecated]] = 20, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Cut instead
        CId_Cut = 20,

        Copy [[deprecated]] =
            21, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Copy instead
        CId_Copy = 21,

        Paste [[deprecated]] =
            22, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Paste instead
        CId_Paste = 22,

        Clear [[deprecated]] =
            23, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Clear instead
        CId_Clear = 23,

        CutSpecial [[deprecated]] =
            24, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CutSpecial instead
        CId_CutSpecial = 24,

        CopySpecial [[deprecated]] =
            25, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CopySpecial instead
        CId_CopySpecial = 25,

        ClearSpecial [[deprecated]] =
            26, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ClearSpecial instead
        CId_ClearSpecial = 26,

        PasteSpecial [[deprecated]] =
            27, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_PasteSpecial instead
        CId_PasteSpecial = 27,

        ExportMix [[deprecated]] =
            28, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ExportMix instead
        CId_ExportMix = 28,

        Spot [[deprecated]] =
            29, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Spot instead
        CId_Spot = 29,

        ExportSessionInfoAsText [[deprecated]] =
            30, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ExportSessionInfoAsText instead
        CId_ExportSessionInfoAsText = 30,

        GetDynamicProperties [[deprecated]] =
            31, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetDynamicProperties instead
        CId_GetDynamicProperties = 31,

        SetPlaybackMode [[deprecated]] =
            32, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetPlaybackMode instead
        CId_SetPlaybackMode = 32,

        SetRecordMode [[deprecated]] =
            33, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetRecordMode instead
        CId_SetRecordMode = 33,

        GetSessionAudioFormat [[deprecated]] =
            34, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionAudioFormat instead
        CId_GetSessionAudioFormat = 34,

        GetSessionSampleRate [[deprecated]] =
            35, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionSampleRate instead
        CId_GetSessionSampleRate = 35,

        GetSessionBitDepth [[deprecated]] =
            36, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionBitDepth instead
        CId_GetSessionBitDepth = 36,

        GetSessionInterleavedState [[deprecated]] =
            37, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionInterleavedState instead
        CId_GetSessionInterleavedState = 37,

        GetSessionTimeCodeRate [[deprecated]] =
            38, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionTimeCodeRate instead
        CId_GetSessionTimeCodeRate = 38,

        GetSessionFeetFramesRate [[deprecated]] =
            39, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionFeetFramesRate instead
        CId_GetSessionFeetFramesRate = 39,

        GetSessionAudioRatePullSettings [[deprecated]] =
            40, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionAudioRatePullSettings instead
        CId_GetSessionAudioRatePullSettings = 40,

        GetSessionVideoRatePullSettings [[deprecated]] =
            41, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionVideoRatePullSettings instead
        CId_GetSessionVideoRatePullSettings = 41,

        GetSessionName [[deprecated]] =
            42, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionName instead
        CId_GetSessionName = 42,

        GetSessionPath [[deprecated]] =
            43, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionPath instead
        CId_GetSessionPath = 43,

        GetSessionStartTime [[deprecated]] =
            44, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionStartTime instead
        CId_GetSessionStartTime = 44,

        GetSessionLength [[deprecated]] =
            45, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionLength instead
        CId_GetSessionLength = 45,

        SetSessionAudioFormat [[deprecated]] =
            46, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionAudioFormat instead
        CId_SetSessionAudioFormat = 46,

        SetSessionBitDepth [[deprecated]] =
            47, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionBitDepth instead
        CId_SetSessionBitDepth = 47,

        SetSessionInterleavedState [[deprecated]] =
            48, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionInterleavedState instead
        CId_SetSessionInterleavedState = 48,

        SetSessionTimeCodeRate [[deprecated]] =
            49, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionTimeCodeRate instead
        CId_SetSessionTimeCodeRate = 49,

        SetSessionFeetFramesRate [[deprecated]] =
            50, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionFeetFramesRate instead
        CId_SetSessionFeetFramesRate = 50,

        SetSessionAudioRatePullSettings [[deprecated]] =
            51, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionAudioRatePullSettings instead
        CId_SetSessionAudioRatePullSettings = 51,

        SetSessionVideoRatePullSettings [[deprecated]] =
            52, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionVideoRatePullSettings instead
        CId_SetSessionVideoRatePullSettings = 52,

        SetSessionStartTime [[deprecated]] =
            53, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionStartTime instead
        CId_SetSessionStartTime = 53,

        SetSessionLength [[deprecated]] =
            54, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSessionLength instead
        CId_SetSessionLength = 54,

        GetPTSLVersion [[deprecated]] =
            55, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetPTSLVersion instead
        CId_GetPTSLVersion = 55,

        GetPlaybackMode [[deprecated]] =
            56, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetPlaybackMode instead
        CId_GetPlaybackMode = 56,

        GetRecordMode [[deprecated]] =
            57, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetRecordMode instead
        CId_GetRecordMode = 57,

        GetTransportArmed [[deprecated]] =
            58, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetTransportArmed instead
        CId_GetTransportArmed = 58,

        GetTransportState [[deprecated]] =
            59, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetTransportState instead
        CId_GetTransportState = 59,

        // AuthorizeConnection = 60 is removed since Pro Tools 2023.12
        // just a reminder to keep this order according to CommandId enum in proto file
        ClearMemoryLocation [[deprecated]] =
            61, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ClearMemoryLocation instead
        CId_ClearMemoryLocation = 61,

        RenameSelectedClip [[deprecated]] =
            62, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RenameSelectedClip instead
        CId_RenameSelectedClip = 62,

        RenameTargetClip [[deprecated]] =
            63, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RenameTargetClip instead
        CId_RenameTargetClip = 63,

        TogglePlayState [[deprecated]] =
            64, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_TogglePlayState instead
        CId_TogglePlayState = 64,

        ToggleRecordEnable [[deprecated]] =
            65, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ToggleRecordEnable instead
        CId_ToggleRecordEnable = 65,

        PlayHalfSpeed [[deprecated]] =
            66, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_PlayHalfSpeed instead
        CId_PlayHalfSpeed = 66,

        RecordHalfSpeed [[deprecated]] =
            67, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RecordHalfSpeed instead
        CId_RecordHalfSpeed = 67,

        EditMemoryLocation [[deprecated]] =
            68, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_EditMemoryLocation instead
        CId_EditMemoryLocation = 68,

        GetMemoryLocations [[deprecated]] =
            69, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetMemoryLocations instead
        CId_GetMemoryLocations = 69,

        RegisterConnection [[deprecated]] =
            70, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RegisterConnection instead
        CId_RegisterConnection = 70,

        CreateMemoryLocation [[deprecated]] =
            71, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CreateMemoryLocation instead
        CId_CreateMemoryLocation = 71,

        CreateNewTracks [[deprecated]] =
            72, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_CreateNewTracks instead
        CId_CreateNewTracks = 72,

        SelectTracksByName [[deprecated]] =
            73, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SelectTracksByName instead
        CId_SelectTracksByName = 73,

        GetEditMode [[deprecated]] =
            74, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetEditMode instead
        CId_GetEditMode = 74,

        SetEditMode [[deprecated]] =
            75, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetEditMode instead
        CId_SetEditMode = 75,

        GetEditTool [[deprecated]] =
            76, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetEditTool instead
        CId_GetEditTool = 76,

        SetEditTool [[deprecated]] =
            77, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetEditTool instead
        CId_SetEditTool = 77,

        RecallZoomPreset [[deprecated]] =
            78, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RecallZoomPreset instead
        CId_RecallZoomPreset = 78,

        GetEditModeOptions [[deprecated]] =
            79, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetEditModeOptions instead
        CId_GetEditModeOptions = 79,

        SetEditModeOptions [[deprecated]] =
            80, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetEditModeOptions instead
        CId_SetEditModeOptions = 80,

        SetTimelineSelection [[deprecated]] =
            81, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTimelineSelection instead
        CId_SetTimelineSelection = 81,

        GetTimelineSelection [[deprecated]] =
            82, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetTimelineSelection instead
        CId_GetTimelineSelection = 82,

        ImportVideo [[deprecated]] =
            83, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ImportVideo instead
        CId_ImportVideo = 83,

        SelectMemoryLocation [[deprecated]] =
            84, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SelectMemoryLocation instead
        CId_SelectMemoryLocation = 84,

        SetTrackMuteState [[deprecated]] =
            85, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackMuteState instead
        CId_SetTrackMuteState = 85,

        SetTrackSoloState [[deprecated]] =
            86, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackSoloState instead
        CId_SetTrackSoloState = 86,

        SetTrackSoloSafeState [[deprecated]] =
            87, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackSoloSafeState instead
        CId_SetTrackSoloSafeState = 87,

        SetTrackRecordEnableState [[deprecated]] =
            88, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackRecordEnableState instead
        CId_SetTrackRecordEnableState = 88,

        SetTrackRecordSafeEnableState [[deprecated]] =
            89, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackRecordSafeEnableState instead
        CId_SetTrackRecordSafeEnableState = 89,

        SetTrackInputMonitorState [[deprecated]] =
            90, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackInputMonitorState instead
        CId_SetTrackInputMonitorState = 90,

        SetTrackSmartDspState [[deprecated]] =
            91, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackSmartDspState instead
        CId_SetTrackSmartDspState = 91,

        SetTrackHiddenState [[deprecated]] =
            92, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackHiddenState instead
        CId_SetTrackHiddenState = 92,

        SetTrackInactiveState [[deprecated]] =
            93, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackInactiveState instead
        CId_SetTrackInactiveState = 93,

        SetTrackFrozenState [[deprecated]] =
            94, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackFrozenState instead
        CId_SetTrackFrozenState = 94,

        SetTrackOnlineState [[deprecated]] =
            95, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackOnlineState instead
        CId_SetTrackOnlineState = 95,

        SetTrackOpenState [[deprecated]] =
            96, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackOpenState instead
        CId_SetTrackOpenState = 96,

        GetSessionIDs [[deprecated]] =
            97, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionIDs instead
        CId_GetSessionIDs = 97,

        GetMemoryLocationsManageMode [[deprecated]] =
            98, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetMemoryLocationsManageMode instead
        CId_GetMemoryLocationsManageMode = 98,

        SetMemoryLocationsManageMode [[deprecated]] =
            99, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetMemoryLocationsManageMode instead
        CId_SetMemoryLocationsManageMode = 99,

        SetMainCounterFormat [[deprecated]] =
            100, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetMainCounterFormat instead
        CId_SetMainCounterFormat = 100,

        SetSubCounterFormat [[deprecated]] =
            101, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetSubCounterFormat instead
        CId_SetSubCounterFormat = 101,

        GetMainCounterFormat [[deprecated]] =
            102, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetMainCounterFormat instead
        CId_GetMainCounterFormat = 102,

        GetSubCounterFormat [[deprecated]] =
            103, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSubCounterFormat instead
        CId_GetSubCounterFormat = 103,

        Undo [[deprecated]] =
            104, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Undo instead
        CId_Undo = 104,

        Redo [[deprecated]] =
            105, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_Redo instead
        CId_Redo = 105,

        UndoAll [[deprecated]] =
            106, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_UndoAll instead
        CId_UndoAll = 106,

        RedoAll [[deprecated]] =
            107, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RedoAll instead
        CId_RedoAll = 107,

        ClearUndoQueue [[deprecated]] =
            108, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ClearUndoQueue instead
        CId_ClearUndoQueue = 108,

        SetTrackDSPModeSafeState [[deprecated]] =
            109, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_SetTrackDSPModeSafeState instead
        CId_SetTrackDSPModeSafeState = 109,

        GetSessionSystemDelayInfo [[deprecated]] =
            110, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GetSessionSystemDelayInfo instead
        CId_GetSessionSystemDelayInfo = 110,

        GroupClips [[deprecated]] =
            111, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_GroupClips instead
        CId_GroupClips = 111,

        UngroupClips [[deprecated]] =
            112, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_UngroupClips instead
        CId_UngroupClips = 112,

        UngroupAllClips [[deprecated]] =
            113, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_UngroupAllClips instead
        CId_UngroupAllClips = 113,

        RegroupClips [[deprecated]] =
            114, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RegroupClips instead
        CId_RegroupClips = 114,

        RepeatSelection [[deprecated]] =
            115, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_RepeatSelection instead
        CId_RepeatSelection = 115,

        DuplicateSelection [[deprecated]] =
            116, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_DuplicateSelection instead
        CId_DuplicateSelection = 116,

        ClearAllMemoryLocations =
            117, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId::CId_ClearAllMemoryLocations instead
        CId_ClearAllMemoryLocations = 117, // Supported starting in Pro Tools 2025.06
        CId_GetTimeAsType = 118,                 // Supported starting in Pro Tools 2025.06
        CId_SubtractLocations = 119,             // Supported starting in Pro Tools 2025.06
        CId_AddLengthToLocation = 120,           // Supported starting in Pro Tools 2025.06
        CId_SubtractPositions = 121,             // Supported starting in Pro Tools 2025.06
        CId_AddLengthToPositions = 122,          // Supported starting in Pro Tools 2025.06
        CId_ImportAudioToClipList = 123,         // Supported starting in Pro Tools 2025.06
        CId_SpotClipsByID = 124,                 // Supported starting in Pro Tools 2025.06
        CId_GetClipList = 125,                   // Supported starting in Pro Tools 2025.06
        CId_GetMediaFileInfo = 126,              // Supported starting in Pro Tools 2025.06
        CId_CreateAudioClips = 127,              // Supported starting in Pro Tools 2025.06
        CId_GetExportMixSourceList = 128,        // Supported starting in Pro Tools 2025.06
        CId_CreateBatchJob = 129,                // Supported starting in Pro Tools 2025.06
        CId_GetMonitorOutputPath = 130,          // Supported starting in Pro Tools 2025.06
        CId_GetEditSelection = 131,              // Supported starting in Pro Tools 2025.06
        CId_SubscribeToEvents = 132,             // Supported starting in Pro Tools 2025.06
        CId_GetBatchJobStatus = 133,             // Supported starting in Pro Tools 2025.06
        CId_BounceTrack = 134,                   // Supported starting in Pro Tools 2025.06
        CId_PollEvents = 135,                    // Supported starting in Pro Tools 2025.06
        CId_UnsubscribeFromEvents = 136,         // Supported starting in Pro Tools 2025.06
        CId_CompleteBatchJob = 137,              // Supported starting in Pro Tools 2025.06
        CId_CancelBatchJob = 138,                // Supported starting in Pro Tools 2025.06
        CId_BeginScrub = 139,                    // Supported starting in Pro Tools 2025.06
        CId_EndScrub = 140,                      // Supported starting in Pro Tools 2025.06
        CId_ContinueScrub = 141,                 // Supported starting in Pro Tools 2025.06
        CId_EnableCueProVideoPlugIn = 142,       // Supported starting in Pro Tools 2025.06
        CId_UpdateVideo = 143,                   // Supported starting in Pro Tools 2025.06
        CId_EnableAPI = 144,                     // Supported starting in Pro Tools 2025.06
        CId_ExchangePublicKeys = 145,            // Supported starting in Pro Tools 2025.06
        CId_CreateSignalPath = 146,              // Supported starting in Pro Tools 2025.06
        CId_SetTrackMainOutputAssignments = 147, // Supported starting in Pro Tools 2025.06
        CId_GetTrackControlInfo = 148,           // Supported starting in Pro Tools 2025.06
        CId_GetTrackControlValue = 149,          // Supported starting in Pro Tools 2025.06
        CId_SetTrackControlValue = 150,          // Supported starting in Pro Tools 2025.06
        CId_InstallMenuHandler = 151,            // Supported starting in Pro Tools 2025.09
        CId_UninstallMenuHandler = 152,          // Supported starting in Pro Tools 2025.09
        CId_SetTrackColor = 153,                 // Supported starting in Pro Tools 2025.09
        CId_GetTrackPlaylists = 154,             // Supported starting in Pro Tools 2025.09
        CId_SetTrackTimebase = 155,              // Supported starting in Pro Tools 2025.09
        CId_GetColorPalette = 156,               // Supported starting in Pro Tools 2025.09
        CId_DeleteTracks = 157,                  // Supported starting in Pro Tools 2025.09
        CId_GetPlaylistElements = 158,           // Supported starting in Pro Tools 2025.09
		CId_WriteSelectedTranscriptionToJSONFile = 159,         // Supported starting in Pro Tools 2025.09
    };

    /**
     * @deprecated PTSLC_CPP::CommandType is deprecated starting in Pro Tools 2025.06, use @ref PTSLC_CPP::CommandId instead
     */
    using CommandType = CommandId;

    /**
     * Class that describes common PTSL exception based on runtime_error.
     */
    class PTSLCommandException : public std::runtime_error
    {
    public:
        /**
         * PTSLCommandException constructor. Used for initialization of PTSLCommandException.
         */
        PTSLCommandException() : std::runtime_error("PTSLCommandException")
        {
        }

        /**
         * PTSLCommandException constructor with parameter. Used for initialization of PTSLCommandException.
         */
        explicit PTSLCommandException(const std::string& message) : std::runtime_error(message)
        {
        }

        /**
         * PTSLCommandException constructor with parameter. Used for initialization of PTSLCommandException.
         */
        explicit PTSLCommandException(const std::string& responseErrorJson, const std::string& commandName,
            CommandId command, const std::string& taskId)
            : std::runtime_error("PTSLCommandException"),
              mResponseErrorJson(responseErrorJson),
              mCommandName(commandName),
              mCommandId(command),
              mTaskId(taskId)
        {
        }

        std::string GetTaskId() const
        {
            return mTaskId;
        }

        CommandId GetCommandId() const
        {
            return mCommandId;
        }

        std::string GetResponseErrorJson() const
        {
            return mResponseErrorJson;
        }

    private:
        CommandId mCommandId;
        std::string mCommandName;
        std::string mTaskId;
        std::string mResponseErrorJson;
    };

    /**
     * Utility method that checks object type. Equivalent of C#'s 'is' operator.
     */
    template <class Dest, class Src>
    bool Is(const Src* exc)
    {
        auto derivedFromSrc = dynamic_cast<const Dest*>(exc);
        return derivedFromSrc != nullptr;
    }

    /**
     * Status of execution of the Pro Tools task.
     */
    enum class TaskStatus : int32_t
    {
        NoResponseReceived [[deprecated]] =
            -1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_NoResponseReceived instead
        TStatus_NoResponseReceived = -1,

        Queued [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_Queued instead
        TStatus_Queued = 0,

        Pending [[deprecated]] =
            1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_Pending instead
        TStatus_Pending = 1,

        InProgress [[deprecated]] =
            2, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_InProgress instead
        TStatus_InProgress = 2,

        Completed [[deprecated]] =
            3, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_Completed instead
        TStatus_Completed = 3,

        Failed [[deprecated]] =
            4, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_Failed instead
        TStatus_Failed = 4,

        WaitingForUserInput [[deprecated]] =
            5, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_WaitingForUserInput instead
        TStatus_WaitingForUserInput = 5,

        CompletedWithBadResponse [[deprecated]] =
            6, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_CompletedWithBadResponse instead
        TStatus_CompletedWithBadResponse = 6,

        FailedWithBadErrorResponse [[deprecated]] =
            7, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus::TStatus_FailedWithBadErrorResponse instead
        TStatus_FailedWithBadErrorResponse = 7
    };

    /**
     * @deprecated PTSLC_CPP::CommandStatusType is deprecated starting in Pro Tools 2025.06, use @ref PTSLC_CPP::TaskStatus instead
     */
    using CommandStatusType = TaskStatus;

    /**
     * Type of the error message which can be returned to user.
     * It can be OS error or Pro Tools error.
     */
    enum class CommandErrorType : int32_t
    {
        CEType_None = -1,

        // Operation System errors
        OS_WritePermissions [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_WritePermissions instead
        CEType_OS_WritePermissions = 0, ///< command hits write permissions

        OS_ErrorCode [[deprecated]] =
            1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_ErrorCode instead
        CEType_OS_ErrorCode = 1, ///< other OS error

        OS_NoLocationFound [[deprecated]] =
            2, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_NoLocationFound instead
        CEType_OS_NoLocationFound = 2, ///< the specified location does not exist

        OS_NoSessionFound [[deprecated]] =
            3, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_NoSessionFound instead
        CEType_OS_NoSessionFound = 3, ///< the specified session does not exist

        OS_FilePathLocation [[deprecated]] =
            4, // @deprecated since Pro Tools 2023.06, use @ref PTSLC_CPP::CommandErrorType::OS_NoFilePathFound instead

        OS_ReadError [[deprecated]] =
            5, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_ReadError instead
        CEType_OS_ReadError = 5, ///< command hits read permissions or can not read specified file

        OS_DiskSpace [[deprecated]] =
            6, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_DiskSpace instead
        CEType_OS_DiskSpace = 6, ///< not enough free space on disk

        OS_DuplicateName [[deprecated]] =
            7, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_DuplicateName instead
        CEType_OS_DuplicateName = 7, ///< the session name to be created is a duplicate of some existing

        OS_IllegalCharacters [[deprecated]] =
            8, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_IllegalCharacters instead
        CEType_OS_IllegalCharacters = 8, ///< the session name contains illegal characters/symbols

        OS_CharactersLimit [[deprecated]] =
            9, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_CharactersLimit instead
        CEType_OS_CharactersLimit = 9, ///< the session/track name is too long (250 characters allowed)

        OS_ProToolsIsNotAvailable [[deprecated]] =
            10, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_ProToolsIsNotAvailable instead
        CEType_OS_ProToolsIsNotAvailable = 10, ///< Pro Tools is not responding during a specified timeout

        OS_NoFilePathFound [[deprecated]] =
            11, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_OS_NoFilePathFound instead
        CEType_OS_NoFilePathFound = 11, ///< the specified file path could not be found

        // Pro Tools errors
        PT_UnknownError [[deprecated]] =
            100, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_UnknownError instead
        CEType_PT_UnknownError = 100, ///< an unspecified error occurred during execution of a command

        PT_NoTemplateGroup [[deprecated]] =
            101, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoTemplateGroup instead
        CEType_PT_NoTemplateGroup = 101, ///< template group does not exist

        PT_NoTemplate [[deprecated]] =
            102, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoTemplate instead
        CEType_PT_NoTemplate = 102, ///< template name does not exist

        PT_SampleRateMismatch [[deprecated]] =
            103, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_SampleRateMismatch instead
        CEType_PT_SampleRateMismatch =
            103, ///< cannot copy/link source media because SRC is on and sample rates don't match

        PT_NoVideoTrackFound [[deprecated]] =
            104, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoVideoTrackFound instead
        CEType_PT_NoVideoTrackFound =
            104, ///< cannot copy/link source media because no video track found in the session

        PT_NoTracksFound [[deprecated]] =
            105, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoTracksFound instead
        CEType_PT_NoTracksFound = 105, ///< warning. no tracks matched because no tracks exist in source

        PT_NoOpenedSession [[deprecated]] =
            106, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoOpenedSession instead
        CEType_PT_NoOpenedSession = 106, ///< session is not open

        PT_NoTrackFound [[deprecated]] =
            107, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoTrackFound instead
        CEType_PT_NoTrackFound = 107, ///< specified track(s) not found

        PT_NoClipsFound [[deprecated]] =
            108, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoClipsFound instead
        CEType_PT_NoClipsFound = 108, ///< specified track contains no clips

        PT_NoSelection [[deprecated]] =
            109, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoSelection instead
        CEType_PT_NoSelection = 109, ///< no selection was found

        PT_RecordDrive [[deprecated]] =
            110, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_RecordDrive instead
        CEType_PT_RecordDrive = 110, ///< the volume is not designated as a record drive

        PT_NoPresetFound [[deprecated]] =
            111, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoPresetFound instead
        CEType_PT_NoPresetFound = 111, ///< preset does not exist in the specified location

        PT_FileTypeMXF [[deprecated]] =
            112, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_FileTypeMXF instead
        CEType_PT_FileTypeMXF = 112, ///< MXF is only available when 'Enforce Media Composer compatibility' is on

        PT_CopyOptionCopy [[deprecated]] =
            113, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_CopyOptionCopy instead
        CEType_PT_CopyOptionCopy =
            113, ///< copy option are unavailable if 'Quantize Edits to Frame Boundaries' is checked

        PT_CopyOptionLink [[deprecated]] =
            114, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_CopyOptionLink instead
        CEType_PT_CopyOptionLink =
            114, ///< link option are unavailable if 'Quantize Edits to Frame Boundaries' is checked

        PT_QuantizeEdits [[deprecated]] =
            115, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_QuantizeEdits instead
        CEType_PT_QuantizeEdits =
            115, ///< 'Quantize Edits to Frame Boundaries' can't be false if 'Enforce Media Composer compatibility' is true

        PT_ExportAsMultichannel [[deprecated]] =
            116, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_ExportAsMultichannel instead
        CEType_PT_ExportAsMultichannel =
            116, ///< warning. 'Export Stereo, 5.1 and 7.1 Tracks as Multichannel' is not applicable because no tracks of that format are contained within the export selection

        PT_IllegalCharactersComments [[deprecated]] =
            117, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_IllegalCharactersComments instead
        CEType_PT_IllegalCharactersComments = 117, ///< comments contain illegal characters

        PT_IllegalCharactersSequenceName [[deprecated]] =
            118, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_IllegalCharactersSequenceName instead
        CEType_PT_IllegalCharactersSequenceName = 118, ///< sequence name contains illegal characters

        PT_MaxCharactersComments [[deprecated]] =
            119, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_MaxCharactersComments instead
        CEType_PT_MaxCharactersComments = 119, ///< comments exceed the maximum character limit

        PT_MaxCharactersSequenceName [[deprecated]] =
            120, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_MaxCharactersSequenceName instead
        CEType_PT_MaxCharactersSequenceName = 120, ///< sequence name exceeds the maximum character limit

        PT_NoSequenceName [[deprecated]] =
            121, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoSequenceName instead
        CEType_PT_NoSequenceName = 121, ///< sequence name can't be empty

        PT_InvalidTask [[deprecated]] =
            122, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_InvalidTask instead
        CEType_PT_InvalidTask = 122, ///< the specified task does not exist

        PT_FileNotFound [[deprecated]] =
            123, //@deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_FileNotFound instead
        CEType_PT_FileNotFound = 123, ///< specified files were not found

        PT_InvalidSelection [[deprecated]] =
            124, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_InvalidSelection instead
        CEType_PT_InvalidSelection = 124, ///< unable to perform action because of invalid selection

        PT_ReadOnlySession [[deprecated]] =
            125, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_ReadOnlySession instead
        CEType_PT_ReadOnlySession =
            125, ///< the session is located on a read-only drive and cannot be saved without user intervention

        PT_InvalidParameter [[deprecated]] =
            126, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_InvalidParameter instead
        CEType_PT_InvalidParameter = 126, ///< One or more parameters are invalid

        PT_Forbidden [[deprecated]] =
            127, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_Forbidden instead
        CEType_PT_Forbidden = 127, ///< operation is not allowed

        PT_NoTimelineFound [[deprecated]] =
            128, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoTimelineFound instead
        CEType_PT_NoTimelineFound = 128, ///< no timeline was found

        PT_ArgumentOutOfRange [[deprecated]] =
            129, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_ArgumentOutOfRange instead
        CEType_PT_ArgumentOutOfRange = 129, ///< argument is out of range

        PT_ForbiddenTrackType [[deprecated]] =
            130, //@deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_ForbiddenTrackType instead
        CEType_PT_ForbiddenTrackType = 130, ///< operation is not allowed for the specified track type

        PT_NoVideoEngineFound [[deprecated]] =
            131, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoVideoEngineFound instead
        CEType_PT_NoVideoEngineFound = 131, ///< video engine is not installed or unavailable

        PT_NoDspHardwareFound [[deprecated]] =
            132, //@deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_NoDspHardwareFound instead
        CEType_PT_NoDspHardwareFound = 132, ///< DSP hardware is not connected or unavailable

        PT_UnsupportedCommand [[deprecated]] =
            133, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_UnsupportedCommand instead
        CEType_PT_UnsupportedCommand = 133, ///< requested command is not supported

        PT_HostNotReady [[deprecated]] =
            134, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_HostNotReady instead
        CEType_PT_HostNotReady = 134, ///< Pro Tools is not fully ready to execute all PTSL commands

        PT_CannotBeDone [[deprecated]] =
            135, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_CannotBeDone instead
        CEType_PT_CannotBeDone =
            135, ///< Operation cannot be done because of some restrictions, limitations or other reasons

        PT_ResponseLengthExceeded [[deprecated]] =
            136, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_ResponseLengthExceeded instead
        CEType_PT_ResponseLengthExceeded = 136, ///< Result message larger than maximum possible gRPC message size

        PT_CommandTimeout [[deprecated]] =
            137, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_CommandTimeout instead
        /*
         * PTSL command was terminated due to timeout
         * Supported starting in Pro Tools 2024.10
         */
        CEType_PT_CommandTimeout = 137,

        PT_HostIsBusy [[deprecated]] =
            138, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_HostIsBusy instead
        /*
         * ProTools is busy with another operation, try again later
         * Supported starting in Pro Tools 2025.06
         */
        CEType_PT_HostIsBusy = 138,

        CEType_PT_DeprecatedParameter = 139, // Supported starting in Pro Tools 2025.06

        CEType_PT_DeprecatedParameterValue = 140, // Supported starting in Pro Tools 2025.06

        CEType_PT_GlobalRendererRequired = 141, // Supported starting in Pro Tools 2025.06

        // Special
        PT_Info [[deprecated]] =
            200, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_PT_Info instead
        CEType_PT_Info = 200, ///< not an error or warning, just some information about command execution


        // SDK errors
        SDK_Error [[deprecated]] =
            -1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_SDK_Error instead
        CEType_SDK_Error =
            -1, ///< Default SDK error used only in Client Wrapper to indicate unknown RPC/Transport issues

        SDK_VersionMismatch [[deprecated]] =
            401, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_SDK_VersionMismatch instead
        CEType_SDK_VersionMismatch = 401, ///< Versions of PTSL Host and PTSL Client are mismatched

        SDK_NotImplemented [[deprecated]] =
            402, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_SDK_NotImplemented instead
        CEType_SDK_NotImplemented = 402, ///< Some PTSL functional is not implemented at the PT side

        SDK_SessionIdParseError [[deprecated]] =
            403, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_SDK_SessionIdParseError instead
        CEType_SDK_SessionIdParseError = 403, ///< Unable to parse the session id from the RegisterConnection response

        SDK_GrpcGeneric [[deprecated]] =
            501, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::CommandErrorType::CEType_SDK_GrpcGeneric instead
        // The 5XX range is reserved for gRPC errors. We can add more specific error codes mapped to real gRPC errors as needed.
        CEType_SDK_GrpcGeneric = 501, ///< Generic gRPC error
    };

    /**
     * Structure that describes common data of command message (used in a response).
     *      
     * @deprecated CommandRequest/CommandResponse and nested structures are deprecated starting in Pro Tools 2024.10 
     * and replaced with CppPTSLRequest/CppPTSLResponse classes.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CommandHeader
    {
        std::string taskId;
        CommandId commandType;
        /**
        * Supported starting in Pro Tools 2024.10
        */
        int32_t version;
        /**
        * Supported starting in Pro Tools 2024.10
        */
        int32_t versionMinor;
        /**
        * Supported starting in Pro Tools 2024.10
        */
        int32_t versionRevision;
    };

    /**
     * Structure that describes command status.
     */
    struct CommandStatus
    {
        TaskStatus type;
        std::string typeClarification;
        int32_t progress;
    };

    /**
     * Structure that describes common data of the error message.
     */
    struct CommandError
    {
        CommandErrorType errorType;
        std::string errorMessage;
        bool isWarning;
    };

    /**
     * Structure that describes error response during command execution.
     */
    struct ResponseError
    {
        std::vector<std::shared_ptr<CommandError>> errors;
    };

    /**
     * Structure that describes common data of the command data.
     * 
     * @deprecated CommandRequest/CommandResponse and nested structures are deprecated starting in Pro Tools 2024.10 
     * and replaced with CppPTSLRequest/CppPTSLResponse classes.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CommandRequest
    {
        CommandId commandType;
        std::string directJsonBody;
    };

    /**
     * Structure that describes common data of the command data.
     * 
     * @deprecated CommandRequest/CommandResponse and nested structures are deprecated starting in Pro Tools 2024.10 
     * and replaced with CppPTSLRequest/CppPTSLResponse classes.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CommandResponse
    {
        CommandHeader header;
        CommandStatus status;
        std::vector<std::shared_ptr<CommandError>> errors;
        std::string jsonBody;
        std::string jsonError;

        virtual ~CommandResponse()
        {
        }

        virtual std::string ToString()
        {
            return std::string();
        }
    };

    /**
     * Structure that describes Pagination request data.
     * 
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] PaginationRequest
    {
        int32_t limit = 0;  // maximum length of the list of data per one response
        int32_t offset = 0; // size of the list of data for current response
    };

    /**
     * Structure that describes GetTaskStatus request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTaskStatusRequest : CommandRequest
    {
        std::string requestedTaskId;
    };

    /**
     * Structure that describes GetTaskStatus response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTaskStatusResponse : CommandResponse
    {
        std::string requestedTaskId;
        CommandStatus requestedTaskStatus;
    };

    /**
     * Type of Pro Tools file.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class FileType : int32_t
    {
        FType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::FileType::WAVE is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileType::FType_WAVE
        */
        WAVE [[deprecated]] = 1,
        FType_WAVE = 1,

        /**
         * @deprecated PTSLC_CPP::FileType::AIFF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileType::FType_AIFF
        */
        AIFF [[deprecated]] = 2,
        FType_AIFF = 2,

        /**
         * @deprecated PTSLC_CPP::FileType::AAF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileType::FType_AAF
        */
        AAF [[deprecated]] = 3,
        FType_AAF = 3,

        /**
         * @deprecated PTSLC_CPP::FileType::OMF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileType::FType_OMF
        */
        OMF [[deprecated]] = 4,
        FType_OMF = 4
    };

    /**
     * Type of Pro Tools session/project sample rate.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SampleRate : int32_t // do not change the order
    {
        SRate_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_None
        */
        SR_None [[deprecated]] = 1,
        SRate_None = 1,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_44100 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_44100
        */
        SR_44100 [[deprecated]] = 44100,
        SRate_44100 = 44100,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_48000 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_48000
        */
        SR_48000 [[deprecated]] = 48000,
        SRate_48000 = 48000,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_88200 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_88200
        */
        SR_88200 [[deprecated]] = 88200,
        SRate_88200 = 88200,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_96000 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_96000
        */
        SR_96000 [[deprecated]] = 96000,
        SRate_96000 = 96000,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_176400 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_176400
        */
        SR_176400 [[deprecated]] = 176400,
        SRate_176400 = 176400,

        /**
         * @deprecated PTSLC_CPP::SampleRate::SR_192000 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SampleRate::SRate_192000
        */
        SR_192000 [[deprecated]] = 192000,
        SRate_192000 = 192000,
    };

    /**
     * Type of Pro Tools input/output settings.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class IOSettings : int32_t
    {
        IOSettings_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_None
        */
        IO_None = 1,
        IOSettings_None = 1,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_Last is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_Last
        */
        IO_Last = 2,
        IOSettings_Last = 2,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_StereoMix is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_StereoMix
        */
        IO_StereoMix = 3,
        IOSettings_StereoMix = 3,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_51FilmMix is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_51FilmMix
        */
        IO_51FilmMix = 4,
        IOSettings_51FilmMix = 4,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_51SMPTEMix is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_51SMPTEMix
        */
        IO_51SMPTEMix = 5,
        IOSettings_51SMPTEMix = 5,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_51DTSMix is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_51DTSMix
        */
        IO_51DTSMix = 6,
        IOSettings_51DTSMix = 6,

        /**
         * @deprecated PTSLC_CPP::IOSettings::IO_UserDefined is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::IOSettings::IOSettings_UserDefined
        */
        IO_UserDefined = 7,
        IOSettings_UserDefined = 7,
    };

    /**
     * Available bit depth for file.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class BitDepth : int32_t
    {
        BDepth_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::BitDepth::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::BitDepth::BDepth_None
        */
        None [[deprecated]] = 1,
        BDepth_None = 1,

        /**
         * @deprecated PTSLC_CPP::BitDepth::Bit16 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::BitDepth::BDepth_16
        */
        Bit16 [[deprecated]] = 2,
        BDepth_16 = 2,

        /**
         * @deprecated PTSLC_CPP::BitDepth::Bit24 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::BitDepth::BDepth_24
        */
        Bit24 [[deprecated]] = 3,
        BDepth_24 = 3,

        /**
         * @deprecated PTSLC_CPP::BitDepth::Bit32Float is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::BitDepth::BDepth_32Float
        */
        Bit32Float [[deprecated]] = 4,
        BDepth_32Float = 4
    };

    /**
     * Structure that describes CreateSession request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CreateSessionRequest : CommandRequest
    {
        std::string sessionName;
        bool createFromTemplate;
        std::string templateGroup;
        std::string templateName;
        FileType fileType;
        SampleRate sampleRate;
        IOSettings inputOutputSettings;
        bool isInterleaved;
        std::string sessionLocation;
        bool isCloudProject;
        bool createFromAAF;
        std::string pathToAAF;
        BitDepth bitDepth;
    };

    /**
     * Structure that describes OpenSession request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] OpenSessionRequest : CommandRequest
    {
        std::string sessionPath;
    };

    /**
     * Types of data which can be imported into Pro Tools.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ImportType : int32_t
    {
        IType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::ImportType::Session is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ImportType::IType_Session
        */
        Session [[deprecated]] = 1,
        IType_Session = 1,

        /**
         * @deprecated PTSLC_CPP::ImportType::Audio is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ImportType::IType_Audio
        */
        Audio [[deprecated]] = 2,
        IType_Audio = 2
    };

    /**
     * Audio import options.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class AudioMediaOptions : int32_t
    {
        AMOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::AudioMediaOptions::LinkToSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioMediaOptions::AMOptions_LinkToSource
        */
        LinkToSource [[deprecated]] = 1,
        AMOptions_LinkToSource = 1,

        /**
         * @deprecated PTSLC_CPP::AudioMediaOptions::CopyFromSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioMediaOptions::AMOptions_CopyFromSource
        */
        CopyFromSource [[deprecated]] = 2,
        AMOptions_CopyFromSource = 2,

        /**
         * @deprecated PTSLC_CPP::AudioMediaOptions::ConsolidateFromSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioMediaOptions::AMOptions_ConsolidateFromSource
        */
        ConsolidateFromSource [[deprecated]] = 3,
        AMOptions_ConsolidateFromSource = 3,

        /**
         * @deprecated PTSLC_CPP::AudioMediaOptions::ForceToTargetSessionFormat is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioMediaOptions::AMOptions_ForceToTargetSessionFormat
        */
        ForceToTargetSessionFormat [[deprecated]] = 4,
        AMOptions_ForceToTargetSessionFormat = 4
    };

    /**
     * Video import options.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class VideoMediaOptions : int32_t
    {
        VMOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::VideoMediaOptions::LinkToSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::VideoMediaOptions::VMOptions_LinkToSourceVideo
        */
        LinkToSource [[deprecated]] = 1,
        VMOptions_LinkToSourceVideo = 1,

        /**
         * @deprecated PTSLC_CPP::VideoMediaOptions::CopyFromSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::VideoMediaOptions::VMOptions_CopyFromSourceVideo
        */
        CopyFromSource [[deprecated]] = 2,
        VMOptions_CopyFromSourceVideo = 2
    };

    /**
     * Options for matching tracks during import.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class MatchTrackOptions : int32_t
    {
        MTOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::MatchTrackOptions::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MatchTrackOptions::MTOptions_None
        */
        None [[deprecated]] = 1,
        MTOptions_None = 1,

        /**
         * @deprecated PTSLC_CPP::MatchTrackOptions::MatchTracks is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MatchTrackOptions::MTOptions_MatchTracks
        */
        MatchTracks [[deprecated]] = 2,
        MTOptions_MatchTracks = 2,

        /**
         * @deprecated PTSLC_CPP::MatchTrackOptions::ImportAsNewTrack is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MatchTrackOptions::MTOptions_ImportAsNewTrack
        */
        ImportAsNewTrack [[deprecated]] = 3,
        MTOptions_ImportAsNewTrack = 3
    };

    /**
     * Options for playlists import.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class MainPlaylistOptions : int32_t
    {
        MPOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::MainPlaylistOptions::ImportReplaceExistingPlaylists is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MainPlaylistOptions::MPOptions_ImportReplaceExistingPlaylists
        */
        ImportReplaceExistingPlaylists [[deprecated]] = 1,
        MPOptions_ImportReplaceExistingPlaylists = 1,

        /**
         * @deprecated PTSLC_CPP::MainPlaylistOptions::ImportOverlayNewOnExistingPlaylists is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MainPlaylistOptions::MPOptions_ImportOverlayNewOnExistingPlaylists
        */
        ImportOverlayNewOnExistingPlaylists [[deprecated]] = 2,
        MPOptions_ImportOverlayNewOnExistingPlaylists = 2,

        /**
         * @deprecated PTSLC_CPP::MainPlaylistOptions::DoNotImport is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MainPlaylistOptions::MPOptions_DoNotImport
        */
        DoNotImport [[deprecated]] = 3,
        MPOptions_DoNotImport = 3
    };

    /**
     * Operations for audio import.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class AudioOperations : int32_t
    {
        AOperations_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::AudioOperations::AddAudio is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioOperations::AOperations_AddAudio
        */
        AddAudio [[deprecated]] = 1,
        AOperations_AddAudio = 1,

        /**
         * @deprecated PTSLC_CPP::AudioOperations::CopyAudio is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioOperations::AOperations_CopyAudio
        */
        CopyAudio [[deprecated]] = 2,
        AOperations_CopyAudio = 2,

        /**
         * @deprecated PTSLC_CPP::AudioOperations::ConvertAudio is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioOperations::AOperations_ConvertAudio
        */
        ConvertAudio [[deprecated]] = 3,
        AOperations_ConvertAudio = 3,

        /**
         * @deprecated PTSLC_CPP::AudioOperations::Default is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AudioOperations::AOperations_Default
        */
        Default [[deprecated]] = 4,
        AOperations_Default = 4
    };

    /**
     * Types of destination.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class MediaDestination : int32_t
    {
        MDestination_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::MediaDestination::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaDestination::MDestination_None
        */
        None [[deprecated]] = 1,
        MDestination_None = 1,

        /**
         * @deprecated PTSLC_CPP::MediaDestination::MainVideoTrack is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaDestination::MDestination_MainVideoTrack
        */
        MainVideoTrack [[deprecated]] = 2,
        MDestination_MainVideoTrack = 2,

        /**
         * @deprecated PTSLC_CPP::MediaDestination::NewTrack is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaDestination::MDestination_NewTrack
        */
        NewTrack [[deprecated]] = 3,
        MDestination_NewTrack = 3,

        /**
         * @deprecated PTSLC_CPP::MediaDestination::ClipList is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaDestination::MDestination_ClipList
        */
        ClipList [[deprecated]] = 4,
        MDestination_ClipList = 4
    };

    /**
     * Types of location in the session.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class MediaLocation : int32_t
    {
        MLocation_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::MediaLocation::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaLocation::MLocation_None
        */
        None [[deprecated]] = 1,
        MLocation_None = 1,

        /**
         * @deprecated PTSLC_CPP::MediaLocation::SessionStart is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaLocation::MLocation_SessionStart
        */
        SessionStart [[deprecated]] = 2,
        MLocation_SessionStart = 2,

        /**
         * @deprecated PTSLC_CPP::MediaLocation::SongStart is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaLocation::MLocation_SongStart
        */
        SongStart [[deprecated]] = 3,
        MLocation_SongStart = 3,

        /**
         * @deprecated PTSLC_CPP::MediaLocation::Selection is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaLocation::MLocation_Selection
        */
        Selection [[deprecated]] = 4,
        MLocation_Selection = 4,

        /**
         * @deprecated PTSLC_CPP::MediaLocation::Spot is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MediaLocation::MLocation_Spot
        */
        Spot [[deprecated]] = 5,
        MLocation_Spot = 5
    };

    /**
     * Type of Pro Tools track.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackType : int32_t
    {
        TType_Unknown = 0,
        TT_Unknown [[deprecated]] =
            0, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Unknown instead
        Unknown [[deprecated]] = 0, // @deprecated since Pro Tools 2023.09

        TType_Midi = 1,
        TT_Midi [[deprecated]] =
            1, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Midi instead
        Midi [[deprecated]] = 1, // @deprecated since Pro Tools 2023.09

        TType_Audio = 2,
        TT_Audio [[deprecated]] =
            2, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Audio instead
        AudioTrack [[deprecated]] = 2, // @deprecated since Pro Tools 2023.09

        TType_Aux = 3,
        TT_Aux [[deprecated]] =
            3,                  // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Aux instead
        Aux [[deprecated]] = 3, // @deprecated since Pro Tools 2023.09

        TType_Video = 4,
        TT_Video [[deprecated]] =
            4, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Video instead
        VideoTrack [[deprecated]] = 4, // @deprecated since Pro Tools 2023.09

        TType_Vca = 5,
        TT_Vca [[deprecated]] =
            5,                  // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Vca instead
        Vca [[deprecated]] = 5, // @deprecated since Pro Tools 2023.09

        /**
         * This track type is included for informational purposes only and has limited functionality in the SDK.
         */
        TType_Tempo = 6,
        TT_Tempo [[deprecated]] =
            6, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Tempo instead
        Tempo [[deprecated]] = 6, // @deprecated since Pro Tools 2023.09

        /**
         * This track type is included for informational purposes only and has limited functionality in the SDK.
         */
        TType_Markers = 7,
        TT_Markers [[deprecated]] =
            7, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Markers instead
        Markers [[deprecated]] = 7, // @deprecated since Pro Tools 2023.09

        /**
         * This track type is included for informational purposes only and has limited functionality in the SDK.
         */
        TType_Meter = 8,
        TT_Meter [[deprecated]] =
            8, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Meter instead
        Meter [[deprecated]] = 8, // @deprecated since Pro Tools 2023.09

        /**
         * This track type is included for informational purposes only and has limited functionality in the SDK.
         */
        TType_KeySignature = 9,
        TT_KeySignature [[deprecated]] =
            9, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_KeySignature instead
        KeySignature [[deprecated]] = 9, // @deprecated since Pro Tools 2023.09

        /**
         * This track type is included for informational purposes only and has limited functionality in the SDK.
         */
        TType_ChordSymbols = 10,
        TT_ChordSymbols [[deprecated]] =
            10, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_ChordSymbols instead
        ChordSymbols [[deprecated]] = 10, // @deprecated since Pro Tools 2023.09

        TType_Instrument = 11,
        TT_Instrument [[deprecated]] =
            11, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Instrument instead
        Instrument [[deprecated]] = 11, // @deprecated since Pro Tools 2023.09

        TType_Master = 12,
        TT_Master [[deprecated]] =
            12, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Master instead
        Master [[deprecated]] = 12, // @deprecated since Pro Tools 2023.09

        /**
         * This track type is included for informational purposes only and has limited functionality in the SDK.
         */
        TType_Heat = 13,
        TT_Heat [[deprecated]] =
            13, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_Heat instead
        Heat [[deprecated]] = 13, // @deprecated since Pro Tools 2023.09

        TType_BasicFolder = 14,
        TT_BasicFolder [[deprecated]] =
            14, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_BasicFolder instead
        BasicFolder [[deprecated]] = 14, // @deprecated since Pro Tools 2023.09

        TType_RoutingFolder = 15,
        TT_RoutingFolder [[deprecated]] =
            15, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_RoutingFolder instead
        RoutingFolder [[deprecated]] = 15, // @deprecated since Pro Tools 2023.09

        TType_CompLane = 16,
        TT_CompLane [[deprecated]] =
            16, // @deprecated since Pro Tools 2025.06, use @ref PTSLC_CPP::TrackType::TType_CompLane instead
        CompLane [[deprecated]] = 16, // @deprecated since Pro Tools 2023.09
    };

    /**
     * Format of Pro Tools track.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackFormat : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_Unknown
        */
        TF_Unknown [[deprecated]] = 0,
        TFormat_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_Mono is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_Mono
        */
        TF_Mono [[deprecated]] = 1,
        TFormat_Mono = 1,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_Stereo is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_Stereo
        */
        TF_Stereo [[deprecated]] = 2,
        TFormat_Stereo = 2,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_LCR is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_LCR
        */
        TF_LCR [[deprecated]] = 3,
        TFormat_LCR = 3,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_LCRS is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_LCRS
        */
        TF_LCRS [[deprecated]] = 4,
        TFormat_LCRS = 4,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_Quad is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_Quad
        */
        TF_Quad [[deprecated]] = 5,
        TFormat_Quad = 5,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5_0 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5_0
        */
        TF_5_0 [[deprecated]] = 6,
        TFormat_5_0 = 6,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5_1 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5_1
        */
        TF_5_1 [[deprecated]] = 7,
        TFormat_5_1 = 7,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5_0_2 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5_0_2
        */
        TF_5_0_2 [[deprecated]] = 8,
        TFormat_5_0_2 = 8,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5_1_2 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5_1_2
        */
        TF_5_1_2 [[deprecated]] = 9,
        TFormat_5_1_2 = 9,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5_0_4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5_0_4
        */
        TF_5_0_4 [[deprecated]] = 10,
        TFormat_5_0_4 = 10,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5_1_4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5_1_4
        */
        TF_5_1_4 [[deprecated]] = 11,
        TFormat_5_1_4 = 11,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_6_0 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_6_0
        */
        TF_6_0 [[deprecated]] = 12,
        TFormat_6_0 = 12,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_6_1 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_6_1
        */
        TF_6_1 [[deprecated]] = 13,
        TFormat_6_1 = 13,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_0 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_0
        */
        TF_7_0 [[deprecated]] = 14,
        TFormat_7_0 = 14,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_1 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_1
        */
        TF_7_1 [[deprecated]] = 15,
        TFormat_7_1 = 15,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_0_SDDS is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_0_SDDS
        */
        TF_7_0_SDDS [[deprecated]] = 16,
        TFormat_7_0_SDDS = 16,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_1_SDDS is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_1_SDDS
        */
        TF_7_1_SDDS [[deprecated]] = 17,
        TFormat_7_1_SDDS = 17,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_0_2 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_0_2
        */
        TF_7_0_2 [[deprecated]] = 18,
        TFormat_7_0_2 = 18,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_1_2 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_1_2
        */
        TF_7_1_2 [[deprecated]] = 19,
        TFormat_7_1_2 = 19,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_0_4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_0_4
        */
        TF_7_0_4 [[deprecated]] = 20,
        TFormat_7_0_4 = 20,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_1_4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_1_4
        */
        TF_7_1_4 [[deprecated]] = 21,
        TFormat_7_1_4 = 21,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_0_6 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_0_6
        */
        TF_7_0_6 [[deprecated]] = 22,
        TFormat_7_0_6 = 22,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7_1_6 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7_1_6
        */
        TF_7_1_6 [[deprecated]] = 23,
        TFormat_7_1_6 = 23,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_9_0_4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_9_0_4
        */
        TF_9_0_4 [[deprecated]] = 24,
        TFormat_9_0_4 = 24,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_9_1_4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_9_1_4
        */
        TF_9_1_4 [[deprecated]] = 25,
        TFormat_9_1_4 = 25,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_9_0_6 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_9_0_6
        */
        TF_9_0_6 [[deprecated]] = 26,
        TFormat_9_0_6 = 26,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_9_1_6 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_9_1_6
        */
        TF_9_1_6 [[deprecated]] = 27,
        TFormat_9_1_6 = 27,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_1stOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_1stOrderAmbisonics
        */
        TF_1stOrderAmbisonics [[deprecated]] = 28,
        TFormat_1stOrderAmbisonics = 28,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_2ndOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_2ndOrderAmbisonics
        */
        TF_2ndOrderAmbisonics [[deprecated]] = 29,
        TFormat_2ndOrderAmbisonics = 29,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_3rdOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_3rdOrderAmbisonics
        */
        TF_3rdOrderAmbisonics [[deprecated]] = 30,
        TFormat_3rdOrderAmbisonics = 30,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_4thOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_4thOrderAmbisonics
        */
        TF_4thOrderAmbisonics [[deprecated]] = 31,
        TFormat_4thOrderAmbisonics = 31,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_5thOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_5thOrderAmbisonics
        */
        TF_5thOrderAmbisonics [[deprecated]] = 32,
        TFormat_5thOrderAmbisonics = 32,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_6thOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_6thOrderAmbisonics
        */
        TF_6thOrderAmbisonics [[deprecated]] = 33,
        TFormat_6thOrderAmbisonics = 33,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_7thOrderAmbisonics is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_7thOrderAmbisonics
        */
        TF_7thOrderAmbisonics [[deprecated]] = 34,
        TFormat_7thOrderAmbisonics = 34,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_None
        */
        TF_None [[deprecated]] = 35,
        TFormat_None = 35,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_2_1 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_2_1
        */
        TF_2_1 [[deprecated]] = 36,
        TFormat_2_1 = 36,

        /**
         * @deprecated PTSLC_CPP::TrackFormat::TF_Overhead is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackFormat::TFormat_Overhead
        */
        TF_Overhead [[deprecated]] = 37,
        TFormat_Overhead = 37,
    };

    /**
     * Timebase of Pro Tools track.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackTimebase : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::TrackTimebase::TTB_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackTimebase::TTimebase_Unknown
        */
        TTB_Unknown [[deprecated]] = 0,
        TTimebase_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TrackTimebase::TTB_Samples is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackTimebase::TTimebase_Samples
        */
        TTB_Samples [[deprecated]] = 1,
        TTimebase_Samples = 1,

        /**
         * @deprecated PTSLC_CPP::TrackTimebase::TTB_Ticks is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackTimebase::TTimebase_Ticks
        */
        TTB_Ticks [[deprecated]] = 2,
        TTimebase_Ticks = 2,

        /**
         * @deprecated PTSLC_CPP::TrackTimebase::TTB_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackTimebase::TTimebase_None
        */
        TTB_None [[deprecated]] = 3,
        TTimebase_None = 3,
    };

    /**
     * Options for time code mapping.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TimeCodeMappingOptions : int32_t
    {
        TCMOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TimeCodeMappingOptions::MaintainAbsoluteTimeCodeValues is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimeCodeMappingOptions::TCMOptions_MaintainAbsoluteTimeCodeValues
        */
        MaintainAbsoluteTimeCodeValues [[deprecated]] = 1,
        TCMOptions_MaintainAbsoluteTimeCodeValues = 1,

        /**
         * @deprecated PTSLC_CPP::TimeCodeMappingOptions::MaintainRelativeTimeCodeValues is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimeCodeMappingOptions::TCMOptions_MaintainRelativeTimeCodeValues
        */
        MaintainRelativeTimeCodeValues [[deprecated]] = 2,
        TCMOptions_MaintainRelativeTimeCodeValues = 2,

        /**
         * @deprecated PTSLC_CPP::TimeCodeMappingOptions::MapStartTimeCodeTo is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimeCodeMappingOptions::TCMOptions_MapStartTimeCodeTo
        */
        MapStartTimeCodeTo [[deprecated]] = 3,
        TCMOptions_MapStartTimeCodeTo = 3
    };

    /**
     * Structure that describes information for audio data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] AudioData
    {
        std::vector<std::string> filesList;
        AudioOperations audioOperations;
        std::string destinationPath;
        MediaDestination destination;
        MediaLocation location;
    };

    /*
     * Options for track offsets.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackOffsetOptions : int32_t
    {
        TOOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TrackOffsetOptions::BarsBeats is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackOffsetOptions::TOOptions_BarsBeats
        */
        BarsBeats [[deprecated]] = 1,
        TOOptions_BarsBeats = 1,

        /**
         * @deprecated PTSLC_CPP::TrackOffsetOptions::MinSecs is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackOffsetOptions::TOOptions_MinSecs
        */
        MinSecs [[deprecated]] = 2,
        TOOptions_MinSecs = 2,

        /**
         * @deprecated PTSLC_CPP::TrackOffsetOptions::TimeCode is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackOffsetOptions::TOOptions_TimeCode
        */
        TimeCode [[deprecated]] = 3,
        TOOptions_TimeCode = 3,

        /**
         * @deprecated PTSLC_CPP::TrackOffsetOptions::FeetFrames is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackOffsetOptions::TOOptions_FeetFrames
        */
        FeetFrames [[deprecated]] = 4,
        TOOptions_FeetFrames = 4,

        /**
         * @deprecated PTSLC_CPP::TrackOffsetOptions::Samples is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackOffsetOptions::TOOptions_Samples
        */
        Samples [[deprecated]] = 5,
        TOOptions_Samples = 5
    };

    /**
     * The types of location for Spot Mode.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SpotLocationType : int32_t
    {
        SLType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SpotLocationType::Start is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SpotLocationType::SLType_Start
        */
        Start [[deprecated]] = 1,
        SLType_Start = 1,

        /**
         * @deprecated PTSLC_CPP::SpotLocationType::SyncPoint is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SpotLocationType::SLType_SyncPoint
        */
        SyncPoint [[deprecated]] = 2,
        SLType_SyncPoint = 2,

        /**
         * @deprecated PTSLC_CPP::SpotLocationType::End is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SpotLocationType::SLType_End
        */
        End [[deprecated]] = 3,
        SLType_End = 3
    };

    /**
     * Types for global timeline times.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TimelineLocationType
    {
        TLType_Unknown,
        TLType_Samples,
        TLType_Ticks,
        TLType_Frames,
        TLType_MinSecs,
        TLType_TimeCode,
        TLType_BarsBeats,
        TLType_FeetFrames,
        TLType_Seconds
    };

    /**
     * Structure that represents a time in the Pro Tools timeline;
     * can be expressed in a variety of units.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] TimelineLocation
    {
        /**
         * Location string, as appropriate for the unit.
         */
        std::string location;

        /**
         * Type of the location time.
         */
        TimelineLocationType timeType;
    };

    /**
     * Structure that describes location data.
     * We sould use this structure also in SpotRequest and ImportRequest.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SpotLocationData
    {
        SpotLocationType locationType;
        std::string locationValue;
        TrackOffsetOptions locationOptions;
    };

    /**
     * Structure that describes Import request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ImportRequest : CommandRequest
    {
        std::string sessionPath;
        ImportType importType;
        AudioMediaOptions audioOptions;
        int32_t audioHandleSize;
        VideoMediaOptions videoOptions;
        MatchTrackOptions matchOptions;
        MainPlaylistOptions playlistOptions;
        std::string trackDataPresetPath;
        bool clipGain;
        bool clipsAndMedia;
        bool volumeAutomation;
        TimeCodeMappingOptions timeCodeMappingUnits;
        std::string timeCodeMappingStartTime;
        bool adjustSessionStartTimeToMatchSource;
        AudioData audioData;

        /** @deprecated
         * locationOptions is deprecated starting in Pro Tools 2023.12
         * Use ImportRequest::SpotLocationData::locationOptions instead
         */
        [[deprecated(
            "Use ImportRequest::SpotLocationData::locationOptions instead")]] TrackOffsetOptions locationOptions;

        /** @deprecated
         * locationType is deprecated starting in Pro Tools 2023.12
         * Use ImportRequest::SpotLocationData::locationType instead
         */
        [[deprecated("Use ImportRequest::SpotLocationData::locationType instead")]] SpotLocationType locationType;

        /** @deprecated
         * locationValue is deprecated starting in Pro Tools 2023.12
         * Use ImportRequest::SpotLocationData::locationValue instead
         */
        [[deprecated("Use ImportRequest::SpotLocationData::locationValue instead")]] std::string locationValue;

        SpotLocationData spotLocationData; // Supported starting in Pro Tools 2023.12
    };

    /**
     * Structure that describes Import response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ImportResponse : CommandResponse
    {
        AudioData audioData;
    };

    /**
     * Type of the state and the way a track attribute is set.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackAttributeState : int32_t
    {
        TAState_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TrackAttributeState::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackAttributeState::TAState_None
        */
        None [[deprecated]] = 1,
        TAState_None = 1,

        /**
         * @deprecated PTSLC_CPP::TrackAttributeState::SetExplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackAttributeState::TAState_SetExplicitly
        */
        SetExplicitly [[deprecated]] = 2,
        TAState_SetExplicitly = 2,

        /**
         * @deprecated PTSLC_CPP::TrackAttributeState::SetImplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackAttributeState::TAState_SetImplicitly
        */
        SetImplicitly [[deprecated]] = 3,
        TAState_SetImplicitly = 3,

        /**
         * @deprecated PTSLC_CPP::TrackAttributeState::SetExplicitlyAndImplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackAttributeState::TAState_SetExplicitlyAndImplicitly
        */
        SetExplicitlyAndImplicitly [[deprecated]] = 4,
        TAState_SetExplicitlyAndImplicitly = 4
    };

    /**
     * Structure that describes Pro Tools track attributes.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] TrackAttributes
    {
        TrackAttributeState isInactive;          // active/inactive implicitly/explicitly
        TrackAttributeState isHidden;            // hidden/shown implicitly/explicitly
        TrackAttributeState isSelected;          // selected implicitly/explicitly
        bool containsClips;                      // contains clips
        bool containsAutomation;                 // contains automation
        bool isSoloed;                           // soloed/unsoloed
        bool isRecordEnabled;                    // record enabled/non-record enabled
        TrackAttributeState isInputMonitoringOn; // input monitoring on/off implicitly/explicitly
        bool isSmartDspOn;                       // smart DSP state (on/off)
        bool isLocked;                           // locked state
        bool isMuted;                            // muted state
        bool isFrozen;                           // frozen state
        bool isOpen;                             // open/closed state
        bool isOnline;                           // provides the status of a video track's Video Online button
        bool isRecordEnabledSafe;
        bool isSmartDspOnSafe;
        bool isSoloedSafe;
    };

    /**
     * Structure that describes Pro Tools track.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] Track
    {
        std::string name;                // track name
        TrackType type;                  // track type
        std::string id;                  // track id
        int32_t index;                   // track index
        std::string color;               // track color (HEX value)
        TrackAttributes trackAttributes; // track attributes
        std::string idCompressed;        // track id	(string as in the GUI)
        TrackFormat format;
        TrackTimebase timebase;
        std::string parentFolderName; // track parent folder name
        std::string parentFolderId;   // track parent folder ID (HEX value)
    };

    /**
     * Types of track list filters.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackListFilter : int32_t
    {
        TLFilter_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::All is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_All
        */
        All [[deprecated]] = 1,
        TLFilter_All = 1,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Selected is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Selected
        */
        Selected [[deprecated]] = 2, // in any way - explicitly or/and implicitly
        TLFilter_Selected = 2,       // in any way - explicitly or/and implicitly

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::SelectedExplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_SelectedExplicitly
        */
        SelectedExplicitly [[deprecated]] = 3,
        TLFilter_SelectedExplicitly = 3,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::SelectedImplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_SelectedImplicitly
        */
        SelectedImplicitly [[deprecated]] = 4,
        TLFilter_SelectedImplicitly = 4,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::WithClipsOnMainPlaylist is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_WithClipsOnMainPlaylist
        */
        WithClipsOnMainPlaylist [[deprecated]] = 5,
        TLFilter_WithClipsOnMainPlaylist = 5,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::WithAutomationOnMainPlaylist is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_WithAutomationOnMainPlaylist
        */
        WithAutomationOnMainPlaylist [[deprecated]] = 6,
        TLFilter_WithAutomationOnMainPlaylist = 6,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Inactive is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Inactive
        */
        Inactive [[deprecated]] = 7, // in any way - explicitly or/and implicitly
        TLFilter_Inactive = 7,       // in any way - explicitly or/and implicitly

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::InactiveExplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_InactiveExplicitly
        */
        InactiveExplicitly [[deprecated]] = 8,
        TLFilter_InactiveExplicitly = 8,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::InactiveImplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_InactiveImplicitly
        */
        InactiveImplicitly [[deprecated]] = 9,
        TLFilter_InactiveImplicitly = 9,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Hidden is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Hidden
        */
        Hidden [[deprecated]] = 10, // in any way - explicitly or/and implicitly
        TLFilter_Hidden = 10,       // in any way - explicitly or/and implicitly

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::HiddenExplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_HiddenExplicitly
        */
        HiddenExplicitly [[deprecated]] = 11,
        TLFilter_HiddenExplicitly = 11,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::HiddenImplicitly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_HiddenImplicitly
        */
        HiddenImplicitly [[deprecated]] = 12,
        TLFilter_HiddenImplicitly = 12,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Locked is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Locked
        */
        Locked [[deprecated]] = 13,
        TLFilter_Locked = 13,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Muted is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Muted
        */
        Muted [[deprecated]] = 14,
        TLFilter_Muted = 14,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Frozen is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Frozen
        */
        Frozen [[deprecated]] = 15,
        TLFilter_Frozen = 15,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Open is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Open
        */
        Open [[deprecated]] = 16,
        TLFilter_Open = 16,

        /**
         * @deprecated PTSLC_CPP::TrackListFilter::Online is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListFilter::TLFilter_Online
        */
        Online [[deprecated]] = 17,
        TLFilter_Online = 17
    };

    /**
     * Structure that describes track list filter that can be inverted (e.g. active => inactive).
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] TrackListInvertibleFilter
    {
        TrackListFilter filter;
        bool isInverted;
    };

    /**
     * Structure that describes GetTrackList request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTrackListRequest : CommandRequest
    {
        PaginationRequest paginationRequest;
        std::vector<TrackListInvertibleFilter> trackFilters;
        bool isFilterListAdditive = true; // TRUE for OR, FALSE for AND
    };

    /**
     * Structure that describes GetTrackList response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTrackListResponse : CommandResponse
    {
        std::vector<Track> trackList;
    };

    /**
     * Structure that describes SelectAllClipsOnTrack request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SelectAllClipsOnTrackRequest : CommandRequest
    {
        std::string trackName;
    };

    /**
     * Structure that describes SelectAllClipsOnTrack error message.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SelectAllClipsOnTrackError : CommandError
    {
        std::string trackName;
    };

    /**
     * Structure that describes ExtendSelectionToTargetTracks request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExtendSelectionToTargetTracksRequest : CommandRequest
    {
        std::vector<std::string> tracksToExtendTo;
    };

    /**
     * Structure that describes ExtendSelectionToTargetTracks error message.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExtendSelectionToTargetTracksError : CommandError
    {
        std::vector<std::string> tracksToExtendTo;
    };

    /**
     * Structure that describes CreateFadesBasedOnPreset request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CreateFadesBasedOnPresetRequest : CommandRequest
    {
        std::string fadePresetName;
        bool autoAdjustBounds = true;
    };

    /**
     * Structure that describes CreateFadesBasedOnPreset error message.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CreateFadesBasedOnPresetError : CommandError
    {
        std::string fadePresetName;
    };

    /**
     * Structure that describes RenameTargetTrack request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RenameTargetTrackRequest : CommandRequest
    {
        /** @deprecated
         * trackId is deprecated starting in Pro Tools 2023.03
         * Use RenameTargetTrackRequest::currentTrackName instead
         */
        [[deprecated("Use RenameTargetTrackRequest::currentTrackName instead")]] std::string trackId;
        std::string currentTrackName;
        std::string newTrackName;
    };

    /**
     * Exported clips format.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ExportFormat : int32_t
    {
        EFormat_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::ExportFormat::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFormat::EFormat_None
        */
        None [[deprecated]] = 1,
        EFormat_None = 1,

        /**
         * @deprecated PTSLC_CPP::ExportFormat::Mono is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFormat::EFormat_Mono
        */
        Mono [[deprecated]] = 2,
        EFormat_Mono = 2,

        /**
         * @deprecated PTSLC_CPP::ExportFormat::MultipleMono is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFormat::EFormat_MultipleMono
        */
        MultipleMono [[deprecated]] = 3,
        EFormat_MultipleMono = 3,

        /**
         * @deprecated PTSLC_CPP::ExportFormat::Interleaved is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFormat::EFormat_Interleaved
        */
        Interleaved [[deprecated]] = 4,
        EFormat_Interleaved = 4
    };

    /**
     * The types of files which the clips will be exported to.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ExportFileType : int32_t
    {
        EFType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::ExportFileType::WAV is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFileType::EFType_WAV
        */
        WAV [[deprecated]] = 1,
        EFType_WAV = 1,

        /**
         * @deprecated PTSLC_CPP::ExportFileType::AIFF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFileType::EFType_AIFF
        */
        AIFF [[deprecated]] = 2,
        EFType_AIFF = 2,

        /**
         * @deprecated PTSLC_CPP::ExportFileType::MXF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportFileType::EFType_MXF
        */
        MXF [[deprecated]] = 3,
        EFType_MXF = 3
    };

    /**
     * Types of possible duplicate names resolution.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ResolveDuplicateNamesBy : int32_t
    {
        RDNBy_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::ResolveDuplicateNamesBy::AutoRenaming is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ResolveDuplicateNamesBy::RDNBy_AutoRenaming
        */
        AutoRenaming [[deprecated]] = 1,
        RDNBy_AutoRenaming = 1,

        /**
         * @deprecated PTSLC_CPP::ResolveDuplicateNamesBy::ReplacingWithNewFiles is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ResolveDuplicateNamesBy::RDNBy_ReplacingWithNewFiles
        */
        ReplacingWithNewFiles [[deprecated]] = 2,
        RDNBy_ReplacingWithNewFiles = 2
    };

    /**
     * Structure that describes ExportClipsAsFiles request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExportClipsAsFilesRequest : CommandRequest
    {
        std::string filePath;
        ExportFormat format;
        ExportFileType fileType;
        BitDepth bitDepth;
        ResolveDuplicateNamesBy duplicateNames;
        bool enforceAvidCompatibility;
        int32_t sampleRateCustom;
    };

    /**
     * The types of files which the tracks will be exported to as AAF/OMF.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ExportAsAAFFileType : int32_t
    {
        EAAFFType_Unknown = 0,
        EAAFFType_None = 1,

        /**
         * @deprecated PTSLC_CPP::ExportAsAAFFileType::WAV is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportAsAAFFileType::EAAFFType_WAV
        */
        WAV [[deprecated]] = 2,
        EAAFFType_WAV = 2,

        /**
         * @deprecated PTSLC_CPP::ExportAsAAFFileType::AIFF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportAsAAFFileType::EAAFFType_AIFF
        */
        AIFF [[deprecated]] = 3,
        EAAFFType_AIFF = 3,

        /**
         * @deprecated PTSLC_CPP::ExportAsAAFFileType::MXF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportAsAAFFileType::EAAFFType_MXF
        */
        MXF [[deprecated]] = 4,
        EAAFFType_MXF = 4,

        /**
         * @deprecated PTSLC_CPP::ExportAsAAFFileType::Embedded is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ExportAsAAFFileType::EAAFFType_Embedded
        */
        Embedded [[deprecated]] = 5,
        EAAFFType_Embedded = 5
    };

    /**
     * Available bit depth for file exported as AAF/OMF.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class AAFFileBitDepth : int32_t
    {
        AAFFBDepth_Unknown = 0,
        AAFFBDepth_None = 1,

        /**
         * @deprecated PTSLC_CPP::AAFFileBitDepth::Bit16 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AAFFileBitDepth::AAFFBDepth_Bit16
        */
        Bit16 [[deprecated]] = 2,
        AAFFBDepth_Bit16 = 2,

        /**
         * @deprecated PTSLC_CPP::AAFFileBitDepth::Bit24 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AAFFileBitDepth::AAFFBDepth_Bit24
        */
        Bit24 [[deprecated]] = 3,
        AAFFBDepth_Bit24 = 3
    };

    /**
     * Available options for copying media while exporting tracks as AAF/OMF files.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class CopyOption : int32_t
    {
        COption_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::CopyOption::ConsolidateFromSourceMedia is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::CopyOption::COption_ConsolidateFromSourceMedia
        */
        ConsolidateFromSourceMedia [[deprecated]] = 1,
        COption_ConsolidateFromSourceMedia = 1,

        /**
         * @deprecated PTSLC_CPP::CopyOption::CopyFromSourceMedia is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::CopyOption::COption_CopyFromSourceMedia
        */
        CopyFromSourceMedia [[deprecated]] = 2,
        COption_CopyFromSourceMedia = 2,

        /**
         * @deprecated PTSLC_CPP::CopyOption::LinkFromSourceMedia is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::CopyOption::COption_LinkFromSourceMedia
        */
        LinkFromSourceMedia [[deprecated]] = 3,
        COption_LinkFromSourceMedia = 3
    };

    /**
     * Structure that describes ExportSelectedTracksAsAAFOMF request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExportSelectedTracksAsAAFOMFRequest : CommandRequest
    {
        ExportAsAAFFileType fileType;
        AAFFileBitDepth bitDepth;
        CopyOption copyOption;
        bool enforceMediaComposerCompatibility;
        bool quantizeEditsToFrameBoundaries;
        bool exportStereoAsMultichannel;
        std::string containerFileName;
        std::string containerFileLocation;
        std::string assetFileLocation;
        std::string comments;
        std::string sequenceName;
    };

    /**
     * Structure that describes RefreshTargetAudioFiles request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RefreshTargetAudioFilesRequest : CommandRequest
    {
        std::vector<std::string> filesList;
    };

    /**
     * Structure that describes RefreshTargetAudioFiles error message.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RefreshTargetAudioFilesError : CommandError
    {
        std::vector<std::string> failureList;
    };

    /**
     * Structure that describes RefreshTargetAudioFiles response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RefreshTargetAudioFilesResponse : CommandResponse
    {
        int32_t successCount;
        int32_t failureCount;
        std::vector<std::string> failureList;
    };

    /**
     * Types of the filters which can be applied for session file gathering.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class FileLocationTypeFilter : int32_t
    {
        FLTFilter_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::All_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_AllFiles
        */
        All_Files [[deprecated]] = 1,
        FLTFilter_AllFiles = 1,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::OnTimeline_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_OnTimelineFiles
        */
        OnTimeline_Files [[deprecated]] = 2,
        FLTFilter_OnTimelineFiles = 2,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::NotOnTimeline_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_NotOnTimelineFiles
        */
        NotOnTimeline_Files [[deprecated]] = 3,
        FLTFilter_NotOnTimelineFiles = 3,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::Online_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_OnlineFiles
        */
        Online_Files [[deprecated]] = 4,
        FLTFilter_OnlineFiles = 4,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::Offline_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_OfflineFiles
        */
        Offline_Files [[deprecated]] = 5,
        FLTFilter_OfflineFiles = 5,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::Audio_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_AudioFiles
        */
        Audio_Files [[deprecated]] = 6,
        FLTFilter_AudioFiles = 6,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::Video_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_VideoFiles
        */
        Video_Files [[deprecated]] = 7,
        FLTFilter_VideoFiles = 7,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::Rendered_Files is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_RenderedFiles
        */
        Rendered_Files [[deprecated]] = 8,
        FLTFilter_RenderedFiles = 8,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::SelectedClipsTimeline is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_SelectedClipsTimeline
        */
        SelectedClipsTimeline [[deprecated]] = 101,
        FLTFilter_SelectedClipsTimeline = 101,

        /**
         * @deprecated PTSLC_CPP::FileLocationTypeFilter::SelectedClipsClipsList is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FileLocationTypeFilter::FLTFilter_SelectedClipsClipsList
        */
        SelectedClipsClipsList [[deprecated]] = 102,
        FLTFilter_SelectedClipsClipsList = 102,

        //Count

        FLTFilter_Count
    };

    /**
     * Structure that describes GetFileLocation request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetFileLocationRequest : CommandRequest
    {
        PaginationRequest paginationRequest;
        std::vector<FileLocationTypeFilter> fileFilters;
    };

    /**
     * Structure that describes file location info.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] FileLocationInfo
    {
        bool isOnline;
    };

    /**
     * Structure that describes file location and its info.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] FileLocation
    {
        std::string path;
        FileLocationInfo info;
    };

    /**
     * Structure that describes GetFileLocation response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetFileLocationResponse : CommandResponse
    {
        std::vector<FileLocation> fileLocations;
    };

    /**
     * Structure that describes CloseSession request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CloseSessionRequest : CommandRequest
    {
        bool saveOnClose;
    };

    /**
     * Structure that describes SaveSessionAs request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SaveSessionAsRequest : CommandRequest
    {
        std::string sessionName;
        std::string sessionLocation;
    };

    /**
     * Automation data options used with cut/copy/clear commands.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class AutomationDataOptions : int32_t
    {
        ADOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::AutomationDataOptions::All_Automation is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AutomationDataOptions::ADOptions_AllAutomation
        */
        All_Automation [[deprecated]] = 1,
        ADOptions_AllAutomation = 1,

        /**
         * @deprecated PTSLC_CPP::AutomationDataOptions::Pan_Automation is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AutomationDataOptions::ADOptions_PanAutomation
        */
        Pan_Automation [[deprecated]] = 2,
        ADOptions_PanAutomation = 2,

        /**
         * @deprecated PTSLC_CPP::AutomationDataOptions::PlugIn_Automation is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AutomationDataOptions::ADOptions_PlugInAutomation
        */
        PlugIn_Automation [[deprecated]] = 3,
        ADOptions_PlugInAutomation = 3,

        /**
         * @deprecated PTSLC_CPP::AutomationDataOptions::Clip_Gain is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AutomationDataOptions::ADOptions_ClipGain
        */
        Clip_Gain [[deprecated]] = 4,
        ADOptions_ClipGain = 4,

        /**
         * @deprecated PTSLC_CPP::AutomationDataOptions::Clip_Effects is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::AutomationDataOptions::ADOptions_ClipEffects
        */
        Clip_Effects [[deprecated]] = 5,
        ADOptions_ClipEffects = 5
    };

    /**
     * Options used with the paste special command.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class PasteSpecialOptions : int32_t
    {
        PSOptions_Unknown = 0,

        Merge [[deprecated]] = 1, // @deprecated starting in Pro Tools 2023.12. Use MergeMidi instead.
        /**
         * @deprecated PTSLC_CPP::PasteSpecialOptions::MergeMidi is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PasteSpecialOptions::PSOptions_MergeMidi
        */
        MergeMidi [[deprecated]] = 1,
        PSOptions_MergeMidi = 1,

        /**
         * @deprecated PTSLC_CPP::PasteSpecialOptions::Repeat_To_Fill_Selection is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PasteSpecialOptions::PSOptions_RepeatToFillSelection
        */
        Repeat_To_Fill_Selection [[deprecated]] = 2,
        PSOptions_RepeatToFillSelection = 2,

        /**
         * @deprecated PTSLC_CPP::PasteSpecialOptions::To_Current_Automation_Type is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PasteSpecialOptions::PSOptions_ToCurrentAutomationType
        */
        To_Current_Automation_Type [[deprecated]] = 3,
        PSOptions_ToCurrentAutomationType = 3,

        /**
         * @deprecated PTSLC_CPP::PasteSpecialOptions::MergeMarkers is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PasteSpecialOptions::PSOptions_MergeMarkers
        */
        MergeMarkers [[deprecated]] = 4,
        PSOptions_MergeMarkers = 4
    };

    /**
     * Structure that describes Cut request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CutSpecialRequest : CommandRequest
    {
        AutomationDataOptions automationDataOption;
    };

    /**
     * Structure that describes Cut request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CopySpecialRequest : CommandRequest
    {
        AutomationDataOptions automationDataOption;
    };

    /**
     * Structure that describes ClearSpecial request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ClearSpecialRequest : CommandRequest
    {
        AutomationDataOptions automationDataOption;
    };

    /**
     * Structure that describes PasteSpecial request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] PasteSpecialRequest : CommandRequest
    {
        PasteSpecialOptions pasteSpecialOption;
    };

    /**
     * Boolean with "unset" value.
     * TODO: C++17 transition std::optional<bool>
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TripleBool : int32_t
    {
        TBool_Unknown = 0,

        TB_None [[deprecated]] = 1, // @deprecated starting in Pro Tools 2025.06
        TBool_None = 1,

        TB_False [[deprecated]] = 2, // @deprecated starting in Pro Tools 2025.06
        TBool_False = 2,

        TB_True [[deprecated]] = 3, // @deprecated starting in Pro Tools 2025.06
        TBool_True = 3
    };

    /**
     * Available source types for mix source.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EM_SourceType : int32_t
    {
        EMSType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EM_SourceType::PhysicalOut is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_SourceType::EMSType_PhysicalOut
        */
        PhysicalOut [[deprecated]] = 1,
        EMSType_PhysicalOut = 1,

        /**
         * @deprecated PTSLC_CPP::EM_SourceType::Bus is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_SourceType::EMSType_Bus
        */
        Bus [[deprecated]] = 2,
        EMSType_Bus = 2,

        /**
         * @deprecated PTSLC_CPP::EM_SourceType::Output is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_SourceType::EMSType_Output
        */
        Output [[deprecated]] = 3,
        EMSType_Output = 3
    };

    /**
     * Available file type for mix export.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EM_FileType : int32_t
    {
        EMFType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_None
        */
        None [[deprecated]] = 1,
        EMFType_None = 1,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::MOV is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_MOV
        */
        MOV [[deprecated]] = 2,
        EMFType_MOV = 2,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::WAV is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_WAV
        */
        WAV [[deprecated]] = 3,
        EMFType_WAV = 3,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::AIFF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_AIFF
        */
        AIFF [[deprecated]] = 4,
        EMFType_AIFF = 4,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::MP3 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_MP3
        */
        MP3 [[deprecated]] = 5,
        EMFType_MP3 = 5,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::MXFOPAtom is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_MXFOPAtom
        */
        MXFOPAtom [[deprecated]] = 6,
        EMFType_MXFOPAtom = 6,

        /**
         * @deprecated PTSLC_CPP::EM_FileType::WAVADM is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileType::EMFType_WAVADM
        */
        WAVADM [[deprecated]] = 7,
        EMFType_WAVADM = 7
    };

    /**
     * Available compression type for audio.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class CompressionType : int32_t
    {
        CType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::CompressionType::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::CompressionType::CType_None
        */
        None [[deprecated]] = 1,
        CType_None = 1,

        /**
         * @deprecated PTSLC_CPP::CompressionType::PCM is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::CompressionType::CType_PCM
        */
        PCM [[deprecated]] = 2, // Currently unchangeable, but it may be in the future
        CType_PCM = 2           // Currently unchangeable, but it may be in the future
    };

    /**
     * Available options for export video.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EM_VideoExportOptions : int32_t
    {
        EMVEOptions_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EM_VideoExportOptions::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_VideoExportOptions::EMVEOptions_None
        */
        None [[deprecated]] = 1,
        EMVEOptions_None = 1,

        /**
         * @deprecated PTSLC_CPP::EM_VideoExportOptions::SameAsSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_VideoExportOptions::EMVEOptions_SameAsSource
        */
        SameAsSource [[deprecated]] = 2,
        EMVEOptions_SameAsSource = 2,

        /**
         * @deprecated PTSLC_CPP::EM_VideoExportOptions::Transcode is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_VideoExportOptions::EMVEOptions_Transcode
        */
        Transcode [[deprecated]] = 3,
        EMVEOptions_Transcode = 3
    };

    /**
     * Available types of file destination.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EM_FileDestination : int32_t
    {
        EMFDestination_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EM_FileDestination::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_FileDestination::EMFDestination_None
        */
        None [[deprecated]] = 1,
        EMFDestination_None = 1,

        /**
         * @deprecated PTSLC_CPP::EM_VideoExportOptions::SessionFolder is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_VideoExportOptions::EMFDestination_SessionFolder
        */
        SessionFolder [[deprecated]] = 2,
        EMFDestination_SessionFolder = 2,

        /**
         * @deprecated PTSLC_CPP::EM_VideoExportOptions::Directory is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_VideoExportOptions::EMFDestination_Directory
        */
        Directory [[deprecated]] = 3,
        EMFDestination_Directory = 3
    };

    /**
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    using FileDestination = EM_FileDestination;

    /**
     * Available delivery formats.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EM_DeliveryFormat : int32_t
    {
        EMDFormat_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EM_DeliveryFormat::None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_DeliveryFormat::EMDFormat_None
        */
        None [[deprecated]] = 1,
        EMDFormat_None = 1,

        /**
         * @deprecated PTSLC_CPP::EM_DeliveryFormat::FilePerMixSource is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_DeliveryFormat::EMDFormat_FilePerMixSource
        */
        FilePerMixSource [[deprecated]] = 2,
        EMDFormat_FilePerMixSource = 2,

        /**
         * @deprecated PTSLC_CPP::EM_DeliveryFormat::SingleFile is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EM_DeliveryFormat::EMDFormat_SingleFile
        */
        SingleFile [[deprecated]] = 3,
        EMDFormat_SingleFile = 3
    };

    /**
     * Structure that describes information for audio section in the Bounce Mix window.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EM_AudioInfo
    {
        CompressionType compressionType;
        ExportFormat exportFormat;
        BitDepth bitDepth;
        SampleRate sampleRate;
        TripleBool padToFrameBoundary;
        EM_DeliveryFormat deliveryFormat;
        /**
         * Custom sample rate to override the sampleRate field. Use non zero values to enable this field.
         *
         * Supported starting in Pro Tools 2024.06
         */
        int32_t sampleRateCustom = 0;
    };

    /**
     * Available data types for dynamic properties.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class DP_ValueTypes : int32_t
    {
        DPVTypes_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_double is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Double
        */
        DP_VT_double [[deprecated]] = 1,
        DPVTypes_Double = 1,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_float is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Float
        */
        DP_VT_float [[deprecated]] = 2,
        DPVTypes_Float = 2,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_int32 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Int32
        */
        DP_VT_int32 [[deprecated]] = 3,
        DPVTypes_Int32 = 3,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_int64 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Int64
        */
        DP_VT_int64 [[deprecated]] = 4,
        DPVTypes_Int64 = 4,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_uint32 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Uint32
        */
        DP_VT_uint32 [[deprecated]] = 5,
        DPVTypes_Uint32 = 5,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_uint64 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Uint64
        */
        DP_VT_uint64 [[deprecated]] = 6,
        DPVTypes_Uint64 = 6,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_sint32 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Sint32
        */
        DP_VT_sint32 [[deprecated]] = 7,
        DPVTypes_Sint32 = 7,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_sint64 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Sint64
        */
        DP_VT_sint64 [[deprecated]] = 8,
        DPVTypes_Sint64 = 8,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_fixed32 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Fixed32
        */
        DP_VT_fixed32 [[deprecated]] = 9,
        DPVTypes_Fixed32 = 9,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_fixed64 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Fixed64
        */
        DP_VT_fixed64 [[deprecated]] = 10,
        DPVTypes_Fixed64 = 10,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_sfixed32 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Sfixed32
        */
        DP_VT_sfixed32 [[deprecated]] = 11,
        DPVTypes_Sfixed32 = 11,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_sfixed64 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Sfixed64
        */
        DP_VT_sfixed64 [[deprecated]] = 12,
        DPVTypes_Sfixed64 = 12,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_bool is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Bool
        */
        DP_VT_bool [[deprecated]] = 13,
        DPVTypes_Bool = 13,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_string is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_String
        */
        DP_VT_string [[deprecated]] = 14,
        DPVTypes_String = 14,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_bytes is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Bytes
        */
        DP_VT_bytes [[deprecated]] = 15,
        DPVTypes_Bytes = 15,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_enum is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Enum
        */
        DP_VT_enum [[deprecated]] = 16,
        DPVTypes_Enum = 16,

        /**
         * @deprecated PTSLC_CPP::DP_ValueTypes::DP_VT_object is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DP_ValueTypes::DPVTypes_Object
        */
        DP_VT_object [[deprecated]] = 17,
        DPVTypes_Object = 17
    };

    /**
     * Structure that describes property for dynamic data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] PropertyDescriptor
    {
        std::string
            name; // (required) name of property field - matches the field name in the message object for the associated dynamic property type
        DP_ValueTypes valueType;                 // (required) type of value expected
        std::string objectType;                  // (optional) // name of message object
        bool required;                           // (required) is the property required
        std::string description;                 // (required) brief description of the property
        std::string units;                       // (optional) expected units for this property if applicable
        std::vector<std::string> acceptedValues; // (optional) - a list of accepted values
        std::string maxValue;                    // (optional)
        std::string minValue;                    // (optional)
    };

    /**
     * Structure that describes container for property.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] PropertyContainer
    {
        std::string name;   // (required) name of property field
        DP_ValueTypes type; // (required) data type of value
        std::string value;  // (required) value to associate with the property field
    };

    /**
     * Structure that describes information for codec in video section for Export Mix.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EM_CodecInfo
    {
        std::string codecName;    // (required)
        std::string propertyList; // (required)
    };

    /**
     * Structure that describes information for video section in the Bounce Mix window.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EM_VideoInfo
    {
        TripleBool includeVideo;
        EM_VideoExportOptions videoExportOptions;
        TripleBool replaceTimeCodeTrack;
        EM_CodecInfo codecInfo;
    };

    /**
     * Structure that describes information for import options if importAfterBounce is true.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EM_ImportOptions
    {
        MediaDestination importDestination;
        MediaLocation importLocation;
        int32_t gapsBetweenClips;
        TripleBool importAudioFromFile;
        TripleBool removeExistingVideoTracks;
        TripleBool removeExistingVideoClips;
        TripleBool clearDestinationVideoTrackPlaylist;
    };

    /**
     * Structure that describes information for location section in the Bounce Mix window.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EM_LocationInfo
    {
        TripleBool importAfterBounce;
        EM_ImportOptions importOptions;
        EM_FileDestination fileDestination;
        /**
         * Depending on the fileDestination value, this field can be either:
         * - directory relative to the session directory (if fileDestination is SessionFolder)
         * - absolute path to a directory (if fileDestination is Directory)
         */
        std::string directory;
    };

    /**
     * Structure that describes information for dolby atmos section in the Bounce Mix window.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EM_DolbyAtmosInfo
    {
        TripleBool firstFrameOfAction; // default to false
        std::string timeCodeValue;
        int32_t frameRate;
        std::string propertyList;
    };

    /**
     * Structure that describes ExportMix request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExportMixRequest : CommandRequest
    {
        std::string presetPath;
        std::string fileName;
        EM_FileType fileType;
        /** @deprecated
         * filesList is deprecated starting in Pro Tools 2023.03
         * Use ExportMixRequest::mixSourceList instead
         */
        [[deprecated("Use ExportMixRequest::mixSourceList instead")]] std::string filesList;
        std::string mixSourceList;

        EM_AudioInfo audioInfo;
        EM_VideoInfo videoInfo;
        EM_LocationInfo locationInfo;
        EM_DolbyAtmosInfo dolbyAtmosInfo;

        TripleBool offlineBounce;
    };

    /**
     * Available types of dynamic properties.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class DynamicPropertyType : int32_t
    {
        DPType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::DynamicPropertyType::DP_EM_CodecInfo is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DynamicPropertyType::DPType_CodecInfo
        */
        DP_EM_CodecInfo [[deprecated]] = 1,
        DPType_CodecInfo = 1,

        /**
         * @deprecated PTSLC_CPP::DynamicPropertyType::DP_EM_DolbyAtmosInfo is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::DynamicPropertyType::DPType_DolbyAtmosInfo
        */
        DP_EM_DolbyAtmosInfo [[deprecated]] = 2,
        DPType_DolbyAtmosInfo = 2
    };

    /**
     * Structure that describes GetDynamicProperties request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetDynamicPropertiesRequest : CommandRequest
    {
        DynamicPropertyType propertyType;
    };

    /**
     * Structure that describes GetDynamicProperties group.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetDynamicPropertiesGroup
    {
        std::vector<PropertyContainer> keyList;
        std::vector<PropertyDescriptor> propertyList;
    };

    /**
     * Structure that describes GetDynamicProperties response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetDynamicPropertiesResponse : CommandResponse
    {
        DynamicPropertyType propertyType;
        std::vector<GetDynamicPropertiesGroup> groupList;
    };

    /**
     * Structure that describes Spot request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SpotRequest : CommandRequest
    {
        /** @deprecated
         * locationOptions is deprecated starting in Pro Tools 2023.12
         * Use SpotRequest::SpotLocationData::locationOptions instead
         */
        [[deprecated("Use SpotRequest::SpotLocationData::locationOptions instead")]] TrackOffsetOptions locationOptions;
        /** @deprecated
         * locationType is deprecated starting in Pro Tools 2023.12
         * Use SpotRequest::SpotLocationData::locationType instead
         */
        [[deprecated("Use SpotRequest::SpotLocationData::locationType instead")]] SpotLocationType locationType;
        /** @deprecated
         * locationValue is deprecated starting in Pro Tools 2023.12
         * Use SpotRequest::SpotLocationData::locationValue instead
         */
        [[deprecated("Use SpotRequest::SpotLocationData::locationValue instead")]] std::string locationValue;

        SpotLocationData spotLocationData; // Supported starting in Pro Tools 2023.12
    };

    /**
     * Available track list types.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackListType : int32_t
    {
        TListType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TrackListType::AllTracks is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListType::TListType_AllTracks
        */
        AllTracks [[deprecated]] = 1,
        TListType_AllTracks = 1,

        /**
         * @deprecated PTSLC_CPP::TrackListType::SelectedTracksOnly is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TrackListType::TListType_SelectedTracksOnly
        */
        SelectedTracksOnly [[deprecated]] = 2,
        TListType_SelectedTracksOnly = 2
    };

    /**
     * Available fade handling types.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class FadeHandlingType : int32_t
    {
        FHType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::FadeHandlingType::ShowCrossfades is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FadeHandlingType::FHType_ShowCrossfades
        */
        ShowCrossfades [[deprecated]] = 1,
        FHType_ShowCrossfades = 1,

        /**
         * @deprecated PTSLC_CPP::FadeHandlingType::DontShowCrossfades is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FadeHandlingType::FHType_DontShowCrossfades
        */
        DontShowCrossfades [[deprecated]] = 2,
        FHType_DontShowCrossfades = 2,

        /**
         * @deprecated PTSLC_CPP::FadeHandlingType::CombineCrossfadedClips is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::FadeHandlingType::FHType_CombineCrossfadedClips
        */
        CombineCrossfadedClips [[deprecated]] = 3,
        FHType_CombineCrossfadedClips = 3
    };

    /**
     * Available file formats.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TextAsFileFormat : int32_t
    {
        TFFormat_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TextAsFileFormat::TextEdit is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TextAsFileFormat::TFFormat_TextEdit
        */
        TextEdit [[deprecated]] = 1,
        TFFormat_TextEdit = 1,

        /**
         * @deprecated PTSLC_CPP::TextAsFileFormat::UTF8 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TextAsFileFormat::TFFormat_UTF8
        */
        UTF8 [[deprecated]] = 2,
        TFFormat_UTF8 = 2
    };

    /**
     * Available output types.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ESI_OutputType : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::ESI_OutputType::ESI_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ESI_OutputType::ESIOType_Unknown
        */
        ESI_Unknown [[deprecated]] = 0,
        ESIOType_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::ESI_OutputType::ESI_File is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ESI_OutputType::ESIOType_File
        */
        ESI_File [[deprecated]] = 1,
        ESIOType_File = 1,

        /**
         * @deprecated PTSLC_CPP::ESI_OutputType::ESI_String is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ESI_OutputType::ESIOType_String
        */
        ESI_String [[deprecated]] = 2,
        ESIOType_String = 2
    };

    /**
     * Structure that describes ExportSessionInfoAsText request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExportSessionInfoAsTextRequest : CommandRequest
    {
        bool includeFileList;
        bool includeClipList;
        bool includeMarkers;
        bool includePluginList;
        bool includeTrackEDLs;
        bool showSubFrames;
        bool includeUserTimestamps;
        TrackListType trackListType;
        FadeHandlingType fadeHandlingType;
        TrackOffsetOptions trackOffsetOptions;
        TextAsFileFormat textAsFileFormat;
        ESI_OutputType outputType;
        std::string outputPath;
    };

    /**
     * Structure that describes ExportSessionInfoAsText response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ExportSessionInfoAsTextResponse : CommandResponse
    {
        std::string sessionInfo;
    };

    /**
     * Types of the playback mode.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class PlaybackMode : int32_t
    {
        PMode_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::PM_PlaybackMode::PM_Normal is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PlaybackMode::PMode_Normal
        */
        PM_Normal [[deprecated]] = 1,
        PMode_Normal = 1,

        /**
         * @deprecated PTSLC_CPP::PM_PlaybackMode::PM_Loop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PlaybackMode::PMode_Loop
        */
        PM_Loop [[deprecated]] = 2,
        PMode_Loop = 2,

        /**
         * @deprecated PTSLC_CPP::PM_PlaybackMode::PM_DynamicTransport is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::PlaybackMode::PMode_DynamicTransport
        */
        PM_DynamicTransport [[deprecated]] = 3,
        PMode_DynamicTransport = 3
    };

    /**
     * @deprecated PM_PlaybackMode is deprecated starting in Pro Tools 2024.10, use @ref PTSLC_CPP::PlaybackMode instead
     */
    using PM_PlaybackMode = PlaybackMode;

    /**
     * Structure that describes SetPlaybackMode request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetPlaybackModeRequest : CommandRequest
    {
        PlaybackMode playbackMode;
    };

    /**
     * Structure that describes SetPlaybackMode response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetPlaybackModeResponse : CommandResponse
    {
        PlaybackMode currentPlaybackMode;
        std::vector<PlaybackMode> playbackModeList;
    };

    /**
     * Types of the record mode.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class RecordMode : int32_t
    {
        RMode_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::RM_RecordMode::RM_Normal is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::RecordMode::RMode_Normal
        */
        RM_Normal [[deprecated]] = 1,
        RMode_Normal = 1,

        /**
         * @deprecated PTSLC_CPP::RM_RecordMode::RM_Loop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::RecordMode::RMode_Loop
        */
        RM_Loop [[deprecated]] = 2,
        RMode_Loop = 2,

        /**
         * @deprecated PTSLC_CPP::RM_RecordMode::RM_Destructive is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::RecordMode::RMode_Destructive
        */
        RM_Destructive [[deprecated]] = 3,
        RMode_Destructive = 3,

        /**
         * @deprecated PTSLC_CPP::RM_RecordMode::RM_QuickPunch is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::RecordMode::RMode_QuickPunch
        */
        RM_QuickPunch [[deprecated]] = 4,
        RMode_QuickPunch = 4,

        /**
         * @deprecated PTSLC_CPP::RM_RecordMode::RM_TrackPunch is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::RecordMode::RMode_TrackPunch
        */
        RM_TrackPunch [[deprecated]] = 5,
        RMode_TrackPunch = 5,

        /**
         * @deprecated PTSLC_CPP::RM_RecordMode::RM_DestructivePunch is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::RecordMode::RMode_DestructivePunch
        */
        RM_DestructivePunch [[deprecated]] = 6,
        RMode_DestructivePunch = 6
    };

    /**
     * @deprecated RM_RecordMode is deprecated starting in Pro Tools 2024.10, use @ref PTSLC_CPP::RecordMode instead
     */
    using RM_RecordMode = RecordMode;

    /**
     * Structure that describes SetRecordMode request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetRecordModeRequest : CommandRequest
    {
        RecordMode recordMode;
        bool recordArmTransport;
    };

    /**
     * Structure that describes SetRecordMode response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetRecordModeResponse : CommandResponse
    {
        RecordMode currentRecordMode;
        std::vector<RecordMode> recordModeList;
    };

    /**
     * Type of session audio file format.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SessionAudioFormat : int32_t
    {
        SAFormat_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SessionAudioFormat::SAF_WAVE is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionAudioFormat::SAFormat_WAVE
        */
        SAF_WAVE [[deprecated]] = 1,
        SAFormat_WAVE = 1,

        /**
         * @deprecated PTSLC_CPP::SessionAudioFormat::SAF_AIFF is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionAudioFormat::SAFormat_AIFF
        */
        SAF_AIFF [[deprecated]] = 2,
        SAFormat_AIFF = 2
    };

    /**
     * Type of session time code rate.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SessionTimeCodeRate : int32_t
    {
        STCRate_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps23976 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps23976
        */
        STCR_Fps23976 [[deprecated]] = 1,
        STCRate_Fps23976 = 1,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps24 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps24
        */
        STCR_Fps24 [[deprecated]] = 2,
        STCRate_Fps24 = 2,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps25 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps25
        */
        STCR_Fps25 [[deprecated]] = 3,
        STCRate_Fps25 = 3,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps2997 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps2997
        */
        STCR_Fps2997 [[deprecated]] = 4,
        STCRate_Fps2997 = 4,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps2997Drop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps2997Drop
        */
        STCR_Fps2997Drop [[deprecated]] = 5,
        STCRate_Fps2997Drop = 5,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps30 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps30
        */
        STCR_Fps30 [[deprecated]] = 6,
        STCRate_Fps30 = 6,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps30Drop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps30Drop
        */
        STCR_Fps30Drop [[deprecated]] = 7,
        STCRate_Fps30Drop = 7,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps47952 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps47952
        */
        STCR_Fps47952 [[deprecated]] = 8,
        STCRate_Fps47952 = 8,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps48 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps48
        */
        STCR_Fps48 [[deprecated]] = 9,
        STCRate_Fps48 = 9,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps50 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps50
        */
        STCR_Fps50 [[deprecated]] = 10,
        STCRate_Fps50 = 10,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps5994 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps5994
        */
        STCR_Fps5994 [[deprecated]] = 11,
        STCRate_Fps5994 = 11,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps5994Drop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps5994Drop
        */
        STCR_Fps5994Drop [[deprecated]] = 12,
        STCRate_Fps5994Drop = 12,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps60 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps60
        */
        STCR_Fps60 [[deprecated]] = 13,
        STCRate_Fps60 = 13,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps60Drop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps60Drop
        */
        STCR_Fps60Drop [[deprecated]] = 14,
        STCRate_Fps60Drop = 14,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps100 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps100
        */
        STCR_Fps100 [[deprecated]] = 15,
        STCRate_Fps100 = 15,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps11988 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps11988
        */
        STCR_Fps11988 [[deprecated]] = 16,
        STCRate_Fps11988 = 16,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps11988Drop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps11988Drop
        */
        STCR_Fps11988Drop [[deprecated]] = 17,
        STCRate_Fps11988Drop = 17,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps120 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps120
        */
        STCR_Fps120 [[deprecated]] = 18,
        STCRate_Fps120 = 18,

        /**
         * @deprecated PTSLC_CPP::SessionTimeCodeRate::STCR_Fps120Drop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionTimeCodeRate::STCRate_Fps120Drop
        */
        STCR_Fps120Drop [[deprecated]] = 19,
        STCRate_Fps120Drop = 19
    };

    /**
     * Type of session feet+frames rate.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SessionFeetFramesRate : int32_t
    {
        SFFRate_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SessionFeetFramesRate::SFFR_Fps23976 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionFeetFramesRate::SFFRate_Fps23976
        */
        SFFR_Fps23976 [[deprecated]] = 1,
        SFFRate_Fps23976 = 1,

        /**
         * @deprecated PTSLC_CPP::SessionFeetFramesRate::SFFR_Fps24 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionFeetFramesRate::SFFRate_Fps24
        */
        SFFR_Fps24 [[deprecated]] = 2,
        SFFRate_Fps24 = 2,

        /**
         * @deprecated PTSLC_CPP::SessionFeetFramesRate::SFFR_Fps25 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionFeetFramesRate::SFFRate_Fps25
        */
        SFFR_Fps25 [[deprecated]] = 3,
        SFFRate_Fps25 = 3,
    };

    /**
     * Type of session audio/video rate pull up/down.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SessionRatePull : int32_t
    {
        SRPull_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_None
        */
        SRP_None [[deprecated]] = 1,
        SRPull_None = 1,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Up01 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Up01
        */
        SRP_Up01 [[deprecated]] = 2,
        SRPull_Up01 = 2,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Down01 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Down01
        */
        SRP_Down01 [[deprecated]] = 3,
        SRPull_Down01 = 3,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Up4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Up4
        */
        SRP_Up4 [[deprecated]] = 4,
        SRPull_Up4 = 4,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Up4Up01 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Up4Up01
        */
        SRP_Up4Up01 [[deprecated]] = 5,
        SRPull_Up4Up01 = 5,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Up4Down01 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Up4Down01
        */
        SRP_Up4Down01 [[deprecated]] = 6,
        SRPull_Up4Down01 = 6,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Down4 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Down4
        */
        SRP_Down4 [[deprecated]] = 7,
        SRPull_Down4 = 7,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Down4Up01 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Down4Up01
        */
        SRP_Down4Up01 [[deprecated]] = 8,
        SRPull_Down4Up01 = 8,

        /**
         * @deprecated PTSLC_CPP::SessionRatePull::SRP_Down4Down01 is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SessionRatePull::SRPull_Down4Down01
        */
        SRP_Down4Down01 [[deprecated]] = 9,
        SRPull_Down4Down01 = 9
    };

    /**
     * Structure that describes GetSessionAudioFormat response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionAudioFormatResponse : CommandResponse
    {
        SessionAudioFormat currentSetting;
        std::vector<SessionAudioFormat> possibleSettings;
    };

    /**
     * Structure that describes GetSessionSampleRate response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionSampleRateResponse : CommandResponse
    {
        SampleRate sampleRate;
    };

    /**
     * Structure that describes GetSessionBitDepth response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionBitDepthResponse : CommandResponse
    {
        BitDepth currentSetting;
        std::vector<BitDepth> possibleSettings;
    };

    /**
     * Structure that describes GetSessionInterleavedState response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionInterleavedStateResponse : CommandResponse
    {
        bool currentSetting;
        std::vector<bool> possibleSettings;
    };

    /**
     * Structure that describes GetSessionTimeCodeRate response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionTimeCodeRateResponse : CommandResponse
    {
        SessionTimeCodeRate currentSetting;
        std::vector<SessionTimeCodeRate> possibleSettings;
    };

    /**
     * Structure that describes GetSessionFeetFramesRate response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionFeetFramesRateResponse : CommandResponse
    {
        SessionFeetFramesRate currentSetting;
        std::vector<SessionFeetFramesRate> possibleSettings;
    };

    /**
     * Structure that describes GetSessionAudioRatePullSettings response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionAudioRatePullSettingsResponse : CommandResponse
    {
        SessionRatePull currentSetting;
        std::vector<SessionRatePull> possibleSettings;
    };

    /**
     * Structure that describes GetSessionVideoRatePullSettings response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionVideoRatePullSettingsResponse : CommandResponse
    {
        SessionRatePull currentSetting;
        std::vector<SessionRatePull> possibleSettings;
    };

    /**
     * Structure that describes GetSessionName response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionNameResponse : CommandResponse
    {
        std::string sessionName;
    };

    /**
     * Structure that describes GetSessionPath response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionPathResponse : CommandResponse
    {
        FileLocation sessionPath;
    };

    /**
     * Structure that describes GetSessionStartTime response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionStartTimeResponse : CommandResponse
    {
        std::string sessionStartTime;
    };

    /**
     * Structure that describes GetSessionLength response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionLengthResponse : CommandResponse
    {
        std::string sessionLength;
    };

    /**
     * Structure that describes SetSessionAudioFormat request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionAudioFormatRequest : CommandRequest
    {
        SessionAudioFormat audioFormat;
    };

    /**
     * Structure that describes SetSessionBitDepth request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionBitDepthRequest : CommandRequest
    {
        BitDepth bitDepth;
    };

    /**
     * Structure that describes SetSessionInterleavedState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionInterleavedStateRequest : CommandRequest
    {
        bool interleavedState;
    };

    /**
     * Structure that describes SetSessionTimeCodeRate request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionTimeCodeRateRequest : CommandRequest
    {
        SessionTimeCodeRate timeCodeRate;
    };

    /**
     * Structure that describes SetSessionFeetFramesRate request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionFeetFramesRateRequest : CommandRequest
    {
        SessionFeetFramesRate feetFramesRate;
    };

    /**
     * Structure that describes SetSessionAudioRatePullSettings request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionAudioRatePullSettingsRequest : CommandRequest
    {
        SessionRatePull audioRatePull;
    };

    /**
     * Structure that describes SetSessionVideoRatePullSettings request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionVideoRatePullSettingsRequest : CommandRequest
    {
        SessionRatePull videoRatePull;
    };

    /**
     * Structure that describes SetSessionStartTime request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionStartTimeRequest : CommandRequest
    {
        std::string sessionStartTime;
        TrackOffsetOptions trackOffsetOpts;
        bool maintainRelativePosition;
    };

    /**
     * Structure that describes SetSessionLength request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSessionLengthRequest : CommandRequest
    {
        std::string sessionLength;
    };

    /**
     * We provide versioning of such kinds of PTSL components.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class VersionedPTSLComponents : int32_t
    {
        VPTSLComponents_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::VersionedPTSLComponents::Host is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::VersionedPTSLComponents::VPTSLComponents_Host
        */
        Host [[deprecated]] = 1,  // grpc request to Server side to get Host version
        VPTSLComponents_Host = 1, // grpc request to Server side to get Host version

        /**
         * @deprecated PTSLC_CPP::VersionedPTSLComponents::Client is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::VersionedPTSLComponents::VPTSLComponents_Client
        */
        Client [[deprecated]] = 2,  // get Client version, Client side implementation
        VPTSLComponents_Client = 2, // get Client version, Client side implementation

        /**
         * @deprecated PTSLC_CPP::VersionedPTSLComponents::ClientAPI is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::VersionedPTSLComponents::VPTSLComponents_ClientAPI
        */
        ClientAPI [[deprecated]] = 3, // get Client's method version, Client side implementation
        VPTSLComponents_ClientAPI = 3 // get Client's method version, Client side implementation
    };

    /**
     * Structure that describes GetPTSLVersion request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetPTSLVersionRequest : CommandRequest
    {
        VersionedPTSLComponents ptslComponent;
    };

    /**
     * Structure that describes GetPTSLVersion response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetPTSLVersionResponse : CommandResponse
    {
        int32_t ptslHostVersion; // grpc response from Server
        std::map<std::string, int32_t>
            apiMethodsVersionsList; // fill on Client side (empty/single/all versions of API methods)
    };

    /**
     * Structure that describes GetPlaybackMode response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetPlaybackModeResponse : CommandResponse
    {
        std::vector<PlaybackMode> currentSettings;
        std::vector<PlaybackMode> possibleSettings;
    };

    /**
     * Structure that describes GetRecordMode response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetRecordModeResponse : CommandResponse
    {
        RecordMode currentSetting;
        std::vector<RecordMode> possibleSettings;
    };

    /**
     * Types of the transport state.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TransportState : int32_t
    {
        TState_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportPlaying is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportPlaying
        */
        TS_TransportPlaying [[deprecated]] = 1,
        TState_TransportPlaying = 1,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportStopped is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportStopped
        */
        TS_TransportStopped [[deprecated]] = 2,
        TState_TransportStopped = 2,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportRecording is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportRecording
        */
        TS_TransportRecording [[deprecated]] = 3,
        TState_TransportRecording = 3,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportPlayingHalfSpeed is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportPlayingHalfSpeed
        */
        TS_TransportPlayingHalfSpeed [[deprecated]] = 4,
        TState_TransportPlayingHalfSpeed = 4,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportRecordingHalfSpeed is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportRecordingHalfSpeed
        */
        TS_TransportRecordingHalfSpeed [[deprecated]] = 5,
        TState_TransportRecordingHalfSpeed = 5,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportFastForward is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportFastForward
        */
        TS_TransportFastForward [[deprecated]] = 6,
        TState_TransportFastForward = 6,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportRewind is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportRewind
        */
        TS_TransportRewind [[deprecated]] = 7,
        TState_TransportRewind = 7,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportScrub is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportScrub
        */
        TS_TransportScrub [[deprecated]] = 8,
        TState_TransportScrub = 8,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportShuttle is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportShuttle
        */
        TS_TransportShuttle [[deprecated]] = 9,
        TState_TransportShuttle = 9,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportPrimed is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportPrimed
        */
        TS_TransportPrimed [[deprecated]] = 10,
        TState_TransportPrimed = 10,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportIsCueing is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportIsCueing
        */
        TS_TransportIsCueing [[deprecated]] = 11,
        TState_TransportIsCueing = 11,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportIsCued is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportIsCued
        */
        TS_TransportIsCued [[deprecated]] = 12,
        TState_TransportIsCued = 12,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportIsCuedForPreview is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportIsCuedForPreview
        */
        TS_TransportIsCuedForPreview [[deprecated]] = 13,
        TState_TransportIsCuedForPreview = 13,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportIsStopping is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportIsStopping
        */
        TS_TransportIsStopping [[deprecated]] = 14,
        TState_TransportIsStopping = 14,

        /**
         * @deprecated PTSLC_CPP::TS_TransportState::TS_TransportIsPreviewing is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TransportState::TState_TransportIsPreviewing
        */
        TS_TransportIsPreviewing [[deprecated]] = 15,
        TState_TransportIsPreviewing = 15
    };

    /**
     * @deprecated TS_TransportState is deprecated starting in Pro Tools 2024.10, use @ref PTSLC_CPP::TransportState instead
     */
    using TS_TransportState = TransportState;

    /**
     * Structure that describes GetTransportState response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTransportStateResponse : CommandResponse
    {
        TransportState currentSetting;
        std::vector<TransportState> possibleSettings;
    };

    /**
     * Structure that describes GetTransportArmed response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTransportArmedResponse : CommandResponse
    {
        bool isTransportArmed;
    };

    /**
     * Structure that describes ClearMemoryLocation request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ClearMemoryLocationRequest : CommandRequest
    {
        /**
         * List of locations that needs to be cleared.
         */
        std::vector<int32_t> locationList;
    };

    /**
     * Structure that describes ClearMemoryLocation error message.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ClearMemoryLocationError : CommandError
    {
        /**
         * List of memory locations that were not cleared.
         */
        std::unordered_set<int64_t> failureList;
    };

    /**
     * Structure that describes ClearMemoryLocation response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ClearMemoryLocationResponse : CommandResponse
    {
        /**
         * Number of succeeded cleared memory locations.
         */
        int32_t successCount;

        /**
         * Number of memory locations that were not cleared.
         */
        int32_t failureCount;

        /**
         * List of memory locations that were not cleared.
         */
        std::unordered_set<int64_t> failureList;
    };

    /**
     * Types of the clip locations.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class ClipLocation : int32_t
    {
        CLocation_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::CL_ClipLocation::CL_ClipsList is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ClipLocation::CLocation_ClipsList
        */
        CL_ClipsList [[deprecated]] = 1,
        CLocation_ClipsList = 1,

        /**
         * @deprecated PTSLC_CPP::CL_ClipLocation::CL_Timeline is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::ClipLocation::CLocation_Timeline
        */
        CL_Timeline [[deprecated]] = 2,
        CLocation_Timeline = 2
    };

    /**
     * @deprecated CL_ClipLocation is deprecated starting in Pro Tools 2024.10, use @ref PTSLC_CPP::ClipLocation instead
     */
    using CL_ClipLocation = ClipLocation;

    /**
     * Structure that describes RenameSelectedClip request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RenameSelectedClipRequest : CommandRequest
    {
        ClipLocation clipLocation;
        std::string newName;
        bool renameFile = true;
        bool renameFileExplicitly = false;
    };

    /**
     * Structure that describes RenameTargetClip request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RenameTargetClipRequest : CommandRequest
    {
        std::string clipName;
        std::string newName;
        bool renameFile = true;
        bool renameFileExplicitly = false;
    };

    /**
     * Type of time properties.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TimeProperties : int32_t
    {
        TProperties_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TimeProperties::TP_Marker is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimeProperties::TProperties_Marker
        */
        TP_Marker [[deprecated]] = 1,
        TProperties_Marker = 1,

        /**
         * @deprecated PTSLC_CPP::TimeProperties::TP_Selection is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimeProperties::TProperties_Selection
        */
        TP_Selection [[deprecated]] = 2,
        TProperties_Selection = 2,

        /**
         * @deprecated PTSLC_CPP::TimeProperties::TP_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimeProperties::TProperties_None
        */
        TP_None [[deprecated]] = 3,
        TProperties_None = 3,
    };

    /**
     * Type of marker location.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class MarkerLocation : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::MarkerLocation::MLC_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MarkerLocation::MarkerLocation_Unknown
        */
        MLC_Unknown [[deprecated]] = 0,
        MarkerLocation_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::MarkerLocation::MLC_MainRuler is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MarkerLocation::MarkerLocation_MainRuler
        */
        MLC_MainRuler [[deprecated]] = 1,
        MarkerLocation_MainRuler = 1,

        /**
         * @deprecated PTSLC_CPP::MarkerLocation::MLC_Track is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MarkerLocation::MarkerLocation_Track
        */
        MLC_Track [[deprecated]] = 2,
        MarkerLocation_Track = 2,
    };

    /**
     * Type of memory location reference.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class MemoryLocationReference : int32_t
    {
        MLReference_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::MemoryLocationReference::MLR_BarBeat is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MemoryLocationReference::MLReference_BarBeat
        */
        MLR_BarBeat [[deprecated]] = 1,
        MLReference_BarBeat = 1,

        /**
         * @deprecated PTSLC_CPP::MemoryLocationReference::MLR_Absolute is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MemoryLocationReference::MLReference_Absolute
        */
        MLR_Absolute [[deprecated]] = 2,
        MLReference_Absolute = 2,

        /**
         * @deprecated PTSLC_CPP::MemoryLocationReference::MLR_FollowTrackTimebase is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MemoryLocationReference::MLReference_FollowTrackTimebase
        */
        MLR_FollowTrackTimebase [[deprecated]] = 3,
        MLReference_FollowTrackTimebase = 3,
    };

    /**
     * Structure that describes location properties (flags).
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] MemoryLocationProperties
    {
        bool zoomSettings;
        bool prePostRollTimes;
        bool trackVisibility;
        bool trackHeights;
        bool groupEnables;
        bool windowConfiguration;
        int32_t windowConfigurationIndex;
        std::string windowConfigurationName;
        int32_t venueSnapshotIndex;
        std::string venueSnapshotName;
    };

    /**
     * Structure that describes memory location.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] MemoryLocation
    {
        int32_t number;
        std::string name;
        std::string startTime;
        std::string endTime;
        TimeProperties timeProperties;
        MemoryLocationReference reference;
        MemoryLocationProperties generalProperties;
        std::string comments;
        MarkerLocation location;
        std::string trackName;
        int32_t colorIndex;
    };

    /**
     * Structure that describes GetMemoryLocation request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetMemoryLocationsRequest : CommandRequest
    {
        PaginationRequest paginationRequest;
    };

    /**
     * Structure that describes EditMemoryLocation request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EditMemoryLocationRequest : CommandRequest
    {
        int32_t number;
        std::string name;
        std::string startTime;
        std::string endTime;
        TimeProperties timeProperties;
        /**
         * Only MLR_FollowTrackTimebase option is supported. All other options are ignored when set.
         * Reference type is determined based on the start_time format instead.
         */
        MemoryLocationReference reference;
        MemoryLocationProperties generalProperties;
        std::string comments;
        int32_t colorIndex;
        MarkerLocation location;
        std::string trackName;
    };

    /**
     * Structure that describes CreateMemoryLocation request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CreateMemoryLocationRequest : CommandRequest
    {
        int32_t number;
        std::string name;
        std::string startTime;
        std::string endTime;
        TimeProperties timeProperties;
        /**
         * Only MLR_FollowTrackTimebase option is supported. All other options are ignored when set.
         * Reference type is determined based on the start_time format instead.
         */
        MemoryLocationReference reference;
        MemoryLocationProperties generalProperties;
        std::string comments;
        int32_t colorIndex;
        MarkerLocation location;
        std::string trackName;
    };

    /**
     * Structure that describes GetMemoryLocations response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetMemoryLocationsResponse : CommandResponse
    {
        std::vector<MemoryLocation> memoryLocations;
    };

    /**
     * Structure that describes RegisterConnection request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RegisterConnectionRequest : CommandRequest
    {
        std::string companyName;
        std::string applicationName;
    };

    /**
     * Structure that describes RegisterConnection response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RegisterConnectionResponse : CommandResponse
    {
        std::string sessionId;
    };

    /**
     * Enumeration that describes the position of the insertion point for creating new tracks.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackInsertionPoint
    {
        /**
         * Default value, the insertion point is determined by Pro Tools.
         */
        TIPoint_Unknown = 0,
        /**
         * The insertion point is before the specified track.
         */
        TIPoint_Before = 1,
        /**
         * The insertion point is after the specified track.
         */
        TIPoint_After = 2,
        /**
         * The insertion point is either the first track in the session or the first member track of the specified folder track.
         */
        TIPoint_First = 3,
        /**
         * The insertion point is either the last track in the session or the last member track of the specified folder track.
         */
        TIPoint_Last = 4
    };

    /**
     * Structure that describes CreateNewTracks request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CreateNewTracksRequest : CommandRequest
    {
        int32_t numberOfTracks { 0 };
        std::string trackName;
        TrackFormat trackFormat { TrackFormat::TF_Unknown };
        TrackType trackType { TrackType::TT_Unknown };
        TrackTimebase trackTimebase { TrackTimebase::TTB_Unknown };
        /**
         * Non-default pagination_request values can be used to limit the response (CreateNewTracksResponseBody.created_track_names)
         *
         * Supported starting in Pro Tools 2024.06
        */
        PaginationRequest paginationRequest;
        /**
         * Supported starting in Pro Tools 2024.06
        */
        TrackInsertionPoint insertionPointPosition { TrackInsertionPoint::TIPoint_Unknown };
        /**
         * Supported starting in Pro Tools 2024.06
        */
        std::string insertionPointTrackName;
    };

    /**
     * Structure that describes CreateNewTracks response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] CreateNewTracksResponse : CommandResponse
    {
        /**
         * @deprecated since Pro Tools 2024.06, use createdTrackNames
         */
        int32_t numberOfTracks { 0 };
        /**
         * Supported starting in Pro Tools 2024.06
        */
        std::vector<std::string> createdTrackNames;
    };

    /**
     * Type of edit mode.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EditMode : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_Unknown
        */
        EMO_Unknown [[deprecated]] = 0,
        EMode_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_Shuffle is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::MemoryLocationReference::EMode_Shuffle
        */
        EMO_Shuffle [[deprecated]] = 1,
        EMode_Shuffle = 1,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_Slip is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_Slip
        */
        EMO_Slip [[deprecated]] = 2,
        EMode_Slip = 2,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_Spot is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_Spot
        */
        EMO_Spot [[deprecated]] = 3,
        EMode_Spot = 3,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_GridAbsolute is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_GridAbsolute
        */
        EMO_GridAbsolute [[deprecated]] = 4,
        EMode_GridAbsolute = 4,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_GridRelative is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_GridRelative
        */
        EMO_GridRelative [[deprecated]] = 5,
        EMode_GridRelative = 5,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_ShuffleSnapToGridAbsolute is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_ShuffleSnapToGridAbsolute
        */
        EMO_ShuffleSnapToGridAbsolute [[deprecated]] = 6,
        EMode_ShuffleSnapToGridAbsolute = 6,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_SlipSnapToGridAbsolute is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_SlipSnapToGridAbsolute
        */
        EMO_SlipSnapToGridAbsolute [[deprecated]] = 7,
        EMode_SlipSnapToGridAbsolute = 7,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_SpotSnapToGridAbsolute is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_SpotSnapToGridAbsolute
        */
        EMO_SpotSnapToGridAbsolute [[deprecated]] = 8,
        EMode_SpotSnapToGridAbsolute = 8,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_ShuffleSnapToGridRelative is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_ShuffleSnapToGridRelative
        */
        EMO_ShuffleSnapToGridRelative [[deprecated]] = 9,
        EMode_ShuffleSnapToGridRelative = 9,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_SlipSnapToGridRelative is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_SlipSnapToGridRelative
        */
        EMO_SlipSnapToGridRelative [[deprecated]] = 10,
        EMode_SlipSnapToGridRelative = 10,

        /**
         * @deprecated PTSLC_CPP::EditMode::EMO_SpotSnapToGridRelative is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditMode::EMode_SpotSnapToGridRelative
        */
        EMO_SpotSnapToGridRelative [[deprecated]] = 11,
        EMode_SpotSnapToGridRelative = 11,
    };

    /**
     * Type of edit tool.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class EditTool : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::EditTool::ET_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_Unknown
        */
        ET_Unknown [[deprecated]] = 0,
        ETool_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_ZoomNormal is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_ZoomNormal
        */
        ET_ZoomNormal [[deprecated]] = 1,
        ETool_ZoomNormal = 1,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_ZoomSingle is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_ZoomSingle
        */
        ET_ZoomSingle [[deprecated]] = 2,
        ETool_ZoomSingle = 2,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_TrimStandard is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_TrimStandard
        */
        ET_TrimStandard [[deprecated]] = 3,
        ETool_TrimStandard = 3,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_TrimTce is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_TrimTce
        */
        ET_TrimTce [[deprecated]] = 4,
        ETool_TrimTce = 4,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_TrimScrub is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_TrimScrub
        */
        ET_TrimScrub [[deprecated]] = 5,
        ETool_TrimScrub = 5,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_TrimLoop is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_TrimLoop
        */
        ET_TrimLoop [[deprecated]] = 6,
        ETool_TrimLoop = 6,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_Selector is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_Selector
        */
        ET_Selector [[deprecated]] = 7,
        ETool_Selector = 7,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_GrabberTime is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_GrabberTime
        */
        ET_GrabberTime [[deprecated]] = 8,
        ETool_GrabberTime = 8,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_GrabberSeparation is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_GrabberSeparation
        */
        ET_GrabberSeparation [[deprecated]] = 9,
        ETool_GrabberSeparation = 9,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_GrabberObject is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_GrabberObject
        */
        ET_GrabberObject [[deprecated]] = 10,
        ETool_GrabberObject = 10,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_SmartTool is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_SmartTool
        */
        ET_SmartTool [[deprecated]] = 11,
        ETool_SmartTool = 11,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_Scrubber is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_Scrubber
        */
        ET_Scrubber [[deprecated]] = 12,
        ETool_Scrubber = 12,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilFreeHand is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilFreeHand
        */
        ET_PencilFreeHand [[deprecated]] = 13,
        ETool_PencilFreeHand = 13,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilLine is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilLine
        */
        ET_PencilLine [[deprecated]] = 14,
        ETool_PencilLine = 14,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilTriangle is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilTriangle
        */
        ET_PencilTriangle [[deprecated]] = 15,
        ETool_PencilTriangle = 15,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilSquare is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilSquare
        */
        ET_PencilSquare [[deprecated]] = 16,
        ETool_PencilSquare = 16,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilRandom is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilRandom
        */
        ET_PencilRandom [[deprecated]] = 17,
        ETool_PencilRandom = 17,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilParabolic is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilParabolic
        */
        ET_PencilParabolic [[deprecated]] = 18,
        ETool_PencilParabolic = 18,

        /**
         * @deprecated PTSLC_CPP::EditTool::ET_PencilSCurve is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::EditTool::ETool_PencilSCurve
        */
        ET_PencilSCurve [[deprecated]] = 19,
        ETool_PencilSCurve = 19,
    };

    /**
     * Structure that describes edit mode options.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] EditModeOptions
    {
        bool tabToTransients;
        bool linkTimelineAndEditSelection;
        bool linkTrackAndEditSelection;
        bool insertionFollowsPlayback;
        bool automationFollowsEdit;
        bool markersFollowEdit;
        bool mirroredMidiEditing;
        bool layeredEditing;
    };

    /**
     * Structure that describes GetEditMode response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetEditModeResponse : CommandResponse
    {
        EditMode currentSetting;
        std::vector<EditMode> possibleSettings;
    };

    /**
     * Structure that describes SetEditMode request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetEditModeRequest : CommandRequest
    {
        EditMode editMode;
    };

    /**
     * Structure that describes GetEditTool response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetEditToolResponse : CommandResponse
    {
        EditTool currentSetting;
        std::vector<EditTool> possibleSettings;
    };

    /**
     * Structure that describes SetEditTool request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetEditToolRequest : CommandRequest
    {
        EditTool editTool;
    };

    /**
     * Structure that describes RecallZoomPreset request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RecallZoomPresetRequest : CommandRequest
    {
        int32_t zoomPreset;
    };

    /**
     * Structure that describes GetEditModeOptions response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetEditModeOptionsResponse : CommandResponse
    {
        EditModeOptions editModeOptions;
    };

    /**
     * Structure that describes SetEditModeOptions request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetEditModeOptionsRequest : CommandRequest
    {
        EditModeOptions editModeOptions;
    };

    /*
     * Type of video synchronization mode.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TimelineUpdateVideo : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::TimelineUpdateVideo::TUV_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimelineUpdateVideo::TUVideo_Unknown
        */
        TUV_Unknown = 0,
        TUVideo_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::TimelineUpdateVideo::TUV_None is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimelineUpdateVideo::TUVideo_None
        */
        TUV_None = 1,
        TUVideo_None = 1,

        /**
         * @deprecated PTSLC_CPP::TimelineUpdateVideo::TUV_In is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimelineUpdateVideo::TUVideo_In
        */
        TUV_In = 2,
        TUVideo_In = 2,

        /**
         * @deprecated PTSLC_CPP::TimelineUpdateVideo::TUV_Out is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::TimelineUpdateVideo::TUVideo_Out
        */
        TUV_Out = 3,
        TUVideo_Out = 3,
    };

    /**
     * Structure that describes SetTimelineSelection request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTimelineSelectionRequest : CommandRequest
    {
        std::string playStartMarkerTime;
        std::string inTime;
        std::string outTime;
        std::string preRollStartTime;
        std::string postRollStopTime;
        TripleBool preRollEnabled { TripleBool::TBool_None };
        TripleBool postRollEnabled { TripleBool::TBool_None };
        TimelineUpdateVideo updateVideoTo { TimelineUpdateVideo::TUVideo_None };
        TripleBool propagateToSatellites { TripleBool::TBool_None };
    };

    /**
     * Structure that describes GetTimelineSelection request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTimelineSelectionRequest : CommandRequest
    {
        TrackOffsetOptions timeScale { TrackOffsetOptions::TOOptions_Samples };
    };

    /**
     * Structure that describes GetTimelineSelection response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetTimelineSelectionResponse : CommandResponse
    {
        std::string playStartMarkerTime;
        std::string inTime;
        std::string outTime;
        std::string preRollStartTime;
        std::string postRollStopTime;
        bool preRollEnabled;
        bool postRollEnabled;
    };

    /**
     * Type of the track's possible selection.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class SelectionMode : int32_t
    {
        /**
         * @deprecated PTSLC_CPP::SelectionMode::SM_Unknown is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SelectionMode::SMode_Unknown
        */
        SM_Unknown [[deprecated]] = 0,
        SMode_Unknown = 0,

        /**
         * @deprecated PTSLC_CPP::SelectionMode::SM_Replace is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SelectionMode::SMode_Replace
        */
        SM_Replace [[deprecated]] = 1,
        SMode_Replace = 1,

        /**
         * @deprecated PTSLC_CPP::SelectionMode::SM_Add is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SelectionMode::SMode_Add
        */
        SM_Add [[deprecated]] = 2,
        SMode_Add = 2,

        /**
         * @deprecated PTSLC_CPP::SelectionMode::SM_Subtract is deprecated starting in Pro Tools 2025.06
         * Replaced with @ref PTSLC_CPP::SelectionMode::SMode_Subtract
        */
        SM_Subtract [[deprecated]] = 3,
        SMode_Subtract = 3,
    };

    /**
     * Structure that describes SelectTracksByName request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SelectTracksByNameRequest : CommandRequest
    {
        PaginationRequest paginationRequest;
        std::vector<std::string> trackNames;
        SelectionMode selectionMode;
    };

    /**
     * Structure that describes SelectTracksByName response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SelectTracksByNameResponse : CommandResponse
    {
        std::vector<Track> selectedTracks;
    };

    /**
     * A file and the failure related to it during importing.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ImportFailureInfo
    {
        std::string filePath;
        std::string failureMessage;
    };

    /**
     * Structure that describes ImportVideo request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ImportVideoRequest : CommandRequest
    {
        std::vector<std::string> videoFilesList;
        MediaDestination destination;
        MediaLocation location;
        SpotLocationData spotLocationData;
        int32_t gapsBetweenClips = 0;
        bool importAudioFromFile = true;
        std::string audioDestinationPath;
        bool removeExistingVideoTracks = false;
        bool removeExistingVideoClips = false;
        bool clearDestinationVideoTrackPlaylist = false;
    };

    /**
     * Structure that describes ImportVideo response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ImportVideoResponse : CommandResponse
    {
        std::vector<ImportFailureInfo> failureList;
    };

    /**
     * Structure that describes SelectMemoryLocation request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SelectMemoryLocationRequest : CommandRequest
    {
        int32_t number;
    };

    /**
     * Structure that describes SetTrackMuteState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackMuteStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackSoloState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackSoloStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackSoloSafeState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackSoloSafeStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackRecordEnableState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackRecordEnableStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackRecordSafeEnableState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackRecordSafeEnableStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackInputMonitorState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackInputMonitorStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackSmartDspState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackSmartDspStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackHiddenState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackHiddenStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackInactiveState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackInactiveStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackFrozenState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackFrozenStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackOnlineState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackOnlineStateRequest : CommandRequest
    {
        std::string trackName;
        bool enabled;
    };

    /**
     * Structure that describes SetTrackOpenState request data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackOpenStateRequest : CommandRequest
    {
        std::vector<std::string> trackNames;
        bool enabled;
    };

    /**
     * Structure that describes GetSessionIDs response data.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionIDsResponse : CommandResponse
    {
        std::string originId;
        std::string instanceId;
        std::string parentId;
    };

    /**
     * Structure that describes GetMemoryLocationsManageMode response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetMemoryLocationsManageModeResponse : CommandResponse
    {
        /**
         * Whether the Memory Locations Manage Mode is enabled or disabled.
         */
        bool enabled;
    };

    /**
     * Structure that describes SetMemoryLocationsManageMode request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetMemoryLocationsManageModeRequest : CommandRequest
    {
        /**
         * Whether to enable or disable the Memory Locations Manage Mode.
         */
        bool enabled;
    };

    /**
     * Structure that describes whether Pro Tools is fully loaded to execute PTSL commands.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] HostReadyCheckResponse : CommandResponse
    {
        bool isHostReady = false;
    };

    /**
     * Structure that describes SetMainCounterFormat request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetMainCounterFormatRequest : CommandRequest
    {
        /**
         * The time scale (type) of the counter.
         */
        TrackOffsetOptions timeScale { TrackOffsetOptions::TOOptions_BarsBeats };
    };

    /**
     * Structure that describes SetSubCounterFormat request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetSubCounterFormatRequest : CommandRequest
    {
        /**
         * The time scale (type) of the counter.
         */
        TrackOffsetOptions timeScale { TrackOffsetOptions::TOOptions_BarsBeats };
    };

    /**
     * Structure that describes GetMainCounterFormat response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetMainCounterFormatResponse : CommandResponse
    {
        /**
         * The time scale (type) of the counter.
         */
        TrackOffsetOptions currentSetting;

        /**
         * The supported time scales (types) of the counter (tier-depended).
         */
        std::vector<TrackOffsetOptions> possibleSettings;
    };

    /**
     * Structure that describes GetSubCounterFormat response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSubCounterFormatResponse : CommandResponse
    {
        /**
         * The time scale (type) of the counter.
         */
        TrackOffsetOptions currentSetting;

        /**
         * The supported time scales (types) of the counter (tier-depended).
         */
        std::vector<TrackOffsetOptions> possibleSettings;
    };

    /**
     * A single Undo/Redo operation (see menu Window -> Undo History in the Pro Tools UI).
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] UndoHistoryOperation
    {
        /**
         * The time of the operation (ISO 8601 format).
         */
        std::chrono::system_clock::time_point time;

        /**
         * The name of the operation.
         */
        std::string operation;

        /**
         * The details of the operation (if available).
         */
        std::string details;
    };

    /**
     * Structure that describes Undo request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] UndoRequest : CommandRequest
    {
        /**
         * The number of levels to undo.
         */
        int32_t levels { 0 };
    };

    /**
     * Structure that describes Undo response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] UndoResponse : CommandResponse
    {
        /**
         * The list of successfully undone operations.
         */
        std::vector<UndoHistoryOperation> operations;
    };

    /**
     * Structure that describes Redo request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RedoRequest : CommandRequest
    {
        /**
         * The number of levels to redo.
         */
        int32_t levels { 0 };
    };

    /**
     * Structure that describes Redo response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RedoResponse : CommandResponse
    {
        /**
         * The list of successfully redone operations.
         */
        std::vector<UndoHistoryOperation> operations;
    };

    /**
     * Structure that describes UndoAll response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] UndoAllResponse : CommandResponse
    {
        /**
         * The list of successfully undone operations.
         */
        std::vector<UndoHistoryOperation> operations;
    };

    /**
     * Structure that describes RedoAll response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RedoAllResponse : CommandResponse
    {
        /**
         * The list of successfully redone operations.
         */
        std::vector<UndoHistoryOperation> operations;
    };

    /**
     * Structure that describes SetTrackDSPModeSafeState request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] SetTrackDSPModeSafeStateRequest : CommandRequest
    {
        /**
         * Tracks to set the DSP Mode Safe state.
         */
        std::vector<std::string> trackNames;

        /**
         * The state to set (`true` for enabling the DSP Mode Safe, `false` for disabling the DSP Mode Safe).
         */
        bool enabled { false };
    };

    /**
     * Structure that describes GetSessionSystemDelayInfo response data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] GetSessionSystemDelayInfoResponse : CommandResponse
    {
        /**
         * The System Delay value of the current session measured in samples.
         */
        int32_t samples;

        /**
         * Indicates whether the Delay Compensation is enabled (`true`) or disabled (`false`).
         */
        bool delayCompensationEnabled;
    };

    /**
     * Possible reasons why clip groups could not be created on specific tracks.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class TrackFromClipGroupExclusionReason
    {
        /**
         * Tech-only value returned when the reason is unknown.
         */
        TCGEReason_Unknown = 0,

        /**
         * Clip groups cannot be created on particular track types (e.g. Aux Input, VCA or Master).
         */
        TCGEReason_TrackTypeIsNotAllowed = 1,

        /**
         * Clip groups cannot be created on frozen tracks.
         */
        TCGEReason_TrackIsFrozen = 2,

        /**
         * Clip groups cannot be created on closed Folder tracks.
         */
        TCGEReason_TrackIsClosed = 3,

        /**
         * Clip groups cannot be created on locked Video tracks.
         */
        TCGEReason_TrackIsLocked = 4,

        /**
         * Clip groups cannot be created on tracks in playlists view.
         */
        TCGEReason_TrackIsInPlaylistView = 5,

        /**
         * Clip groups with embedded fades are not supported on Elastic Audio-enabled tracks.
         */
        TCGEReason_TrackIsElasticAudioWithFades = 6
    };

    /**
     * Structure that describes a track on which the clip group could not be created.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] TrackExcludedFromClipGroupInfo
    {
        /**
         * The name of the track.
         */
        std::string trackName;

        /**
         * The reason why the clip group could not be created on a particular track.
         */
        TrackFromClipGroupExclusionReason reason;
    };

    /**
     * Structure that describes RepeatSelection request data.
     *
     * Supported starting in Pro Tools 2024.06
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] RepeatSelectionRequest : CommandRequest
    {
        /**
         * The number of repeats to repeat selection.
         */
        int32_t numRepeats { 1 };
    };

    /**
     * One of the fundamental time units.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    enum class BasicTimeType
    {
        BTType_Unknown,
        BTType_Samples,
        BTType_Ticks,
        BTType_Frames
    };

    /**
     * Structure that represents an internal position within a piece of media.
     * Not relative to the global timeline. Can only be expressed in fundamental units, depending on media type.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] MediaTimePosition
    {
        /**
         * Position, as appropriate for the unit.
         */
        int64_t position;

        /**
        * Type of the position time.
        */
        BasicTimeType timeType;
    };

    /**
     * Structure that represents a typed relative time position - can apply to both media and timeline times.
     * Not relative to the global timeline. Can only be expressed in fundamental units, depending on media type.
     *
     * @deprecated All API-specific types in the C++ client lib are deprecated starting in Pro Tools 2024.10
     * since we introduced general JSON-based request/response.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] TimeLength
    {
        /**
         * Length, as appropriate for the unit. Can be positive or negative.
         */
        int64_t length;

        /**
        * Type of the location time.
        */
        BasicTimeType timeType;
    };

    /**
     * Structure that describes ClearAllMemoryLocations response data.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ClearAllMemoryLocationsResponse : CommandResponse
    {
        /**
         * The number of successfully cleared memory locations.
         */
        int32_t successCount;

        /**
         * The number of memory locations which were not cleared.
         */
        int32_t failureCount;

        /**
         * The list of memory locations which were not cleared.
         */
        std::vector<int32_t> failureList;
    };

    /**
     * Structure that describes ClearAllMemoryLocations error message.
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] ClearAllMemoryLocationsError : CommandError
    {
        /**
         * List of memory locations which were not cleared.
         */
        std::unordered_set<int64_t> failureList;
    };

} // namespace PTSLC_CPP
