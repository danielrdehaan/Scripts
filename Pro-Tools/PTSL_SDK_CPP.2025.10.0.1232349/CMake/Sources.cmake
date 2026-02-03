# Copyright 2022-2025 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

set(LIBRARY_EXPORT_HEADER "${GENERATED_FILES_DIRECTORY}/PtslCCppExport.h")

list(APPEND PUBLIC_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppAsync.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppCryptoUtils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLClient.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLCommon.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLCommonConversions.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLRequest.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLResponse.h"
    "${LIBRARY_EXPORT_HEADER}"
    )

list(APPEND PRIVATE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLC_DefaultRequest.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLClientInternal.h"
    )

if (PTSLC_CPP_DEVMODE)
    list(APPEND PRIVATE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../../Common/CPP/Source/PTSL_Versions.h")
else()
    list(APPEND PRIVATE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Source/PTSL_Versions.h")
endif()

set(HEADERS ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})

list(APPEND SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppAsync.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppCryptoUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLClient.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLC_DefaultRequest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLCommonConversions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLRequest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CppPTSLResponse.cpp"
    )

list(APPEND COMMANDS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Clear.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ClearSpecial.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CloseSession.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ConsolidateClip.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Copy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CopySpecial.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CreateFade.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CreateSession.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Cut.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CutSpecial.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ExportClipsAsFiles.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ExportMix.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ExportSelectedTracksAsAAFOMF.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ExportSessionInfoAsText.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ExtendSelectionToTargetTracks.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetDynamicProperties.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetFileLocation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetPTSLVersion.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionAudioFormat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionAudioRatePullSettings.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionBitDepth.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionFeetFramesRate.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionInterleavedState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionLength.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionName.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionPath.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionSampleRate.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionStartTime.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionTimeCodeRate.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionVideoRatePullSettings.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetTaskStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetTrackList.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Import.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_OpenSession.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Paste.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_PasteSpecial.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RefreshAllModifiedAudioFiles.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RefreshTargetAudioFiles.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RenameTrack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SaveSession.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SaveSessionAs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SelectAllClipsOnTrack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetPlaybackMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetRecordMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionAudioFormat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionAudioRatePullSettings.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionBitDepth.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionFeetFramesRate.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionInterleavedState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionLength.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionStartTime.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionTimeCodeRate.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSessionVideoRatePullSettings.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Spot.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_TrimToSelection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetPlaybackMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetRecordMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetTransportArmed.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetTransportState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ClearMemoryLocation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RenameSelectedClip.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RenameTargetClip.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_TogglePlayState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ToggleRecordEnable.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_PlayHalfSpeed.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RecordHalfSpeed.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_EditMemoryLocation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CreateMemoryLocation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetMemoryLocations.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RegisterConnection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CreateFadesBasedOnPreset.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RenameTargetTrack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_CreateNewTracks.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetEditMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetEditMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetEditTool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetEditTool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RecallZoomPreset.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetEditModeOptions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetEditModeOptions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTimelineSelection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetTimelineSelection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SelectTracksByName.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ImportVideo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SelectMemoryLocation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackMuteState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackSoloState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackSoloSafeState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackRecordEnableState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackRecordSafeEnableState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackInputMonitorState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackSmartDspState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackHiddenState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackInactiveState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackFrozenState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackOnlineState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackOpenState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionIDs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetMemoryLocationsManageMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetMemoryLocationsManageMode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_HostReadyCheck.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetMainCounterFormat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetSubCounterFormat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetMainCounterFormat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSubCounterFormat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Undo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_Redo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_UndoAll.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RedoAll.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ClearUndoQueue.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_SetTrackDSPModeSafeState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GetSessionSystemDelayInfo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_GroupClips.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_UngroupClips.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_UngroupAllClips.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RegroupClips.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_RepeatSelection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_DuplicateSelection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Commands/CppPTSLC_ClearAllMemoryLocations.cpp"
    )

if (PTSLC_CPP_DEVMODE)
    set(GRPC_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../gRPC")
else()
    set(GRPC_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
endif()

list(APPEND PROTO_FILES
        "${GRPC_SRC_DIR}/PTSL.proto"
        "${GRPC_SRC_DIR}/PTSL.2024.10.0.proto"
        "${GRPC_SRC_DIR}/PTSL.2024.06.0.proto"
    )

set(PLIST_TEMPLATE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist.in")
set(CMAKE_CONFIG_TEMPLATE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Config.cmake.in")

set(UNEXPORTED_SYMBOLS_MAP "${GENERATED_FILES_DIRECTORY}/UnexportedSymbolsList.map")

if(PTSLC_CPP_DEVMODE)
    set(CMAKE_COMMON_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Common/CPP/CMake")
else()
    set(CMAKE_COMMON_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
endif()
