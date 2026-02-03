// Copyright 2020-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief C++ PTSL client implementation.
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <thread>

#include "CppAsync.h"
#include "CppPTSLCommon.h"
#include "CppPTSLRequest.h"
#include "CppPTSLResponse.h"
#include "PtslCCppExport.h"

namespace PTSLC_CPP
{
#if defined(_WIN32)
    /**
     * Export required template class specialization which will be use in the client code.
     */
    template class PTSLC_CPP_EXPORT std::function<void(std::shared_ptr<CommandResponse>)>;
#endif

    /**
     * @deprecated Deprecated starting in Pro Tools 2024.10
     */
    struct [[deprecated("Deprecated starting in 2024.10.")]] DefaultRequestHandler;

    /**
     * Async C++ client wrapper for handling gRPC async streaming requests and receiving responses.
     */
    class PTSLC_CPP_EXPORT CppPTSLClient
    {
    public:
        /**
         * Client's callback event for returning responses on requests. (Current Version)
         */
        std::function<void(std::shared_ptr<CppPTSLResponse>)> OnResponseReceived;

        /**
         * Client's callback event for returning responses on requests. (Legacy Version)
         * @deprecated This version of the function is deprecated and may be removed in future versions.
         * Users are encouraged to use the OnResponseReceived function instead.
         */
        [[deprecated("This version of the function is deprecated. Please use OnResponseReceived instead.")]] std::
            function<void(std::shared_ptr<CommandResponse>)>
                OnResponseReceived_V1_Deprecated;

        /**
         * Client constructor
         * 
         * Used for initialization of gRPC Client and client's config.
         */
        explicit CppPTSLClient(const ClientConfig& config);

        /**
         * Client destructor
         */
        virtual ~CppPTSLClient();

        /**
         * Launches the Pro Tools application
         */
        virtual bool LaunchProTools();

        /**
         * Internal common method for handling different types of requests and responses.
         *
         * @deprecated MakeRequest is deprecated starting in Pro Tools 2024.10.
         * Replaced with @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest"
         */
        [[deprecated("Use SendRequest instead.")]] void MakeRequest(
            std::shared_ptr<DefaultRequestHandler> handler, CommandId commandType);

        /**
         * Internal common method for handling different types of requests and responses via grpc async streaming.
         *
         * @deprecated MakeStreamingRequest is deprecated starting in Pro Tools 2024.10.
         * Replaced with @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest"
         */
        [[deprecated("Use SendRequest instead.")]] void MakeStreamingRequest(
            std::shared_ptr<DefaultRequestHandler> handler, CommandId commandType);

        /**
         * Returns the id of the established session.
         */
        std::string GetSessionId() const;

        /**
         * Sets the session id manually if the session was already established before with @ref PTSLC_CPP::CppPTSLClient::RegisterConnection "RegisterConnection".
         */
        void SetSessionId(const std::string& sessionId);

        /**
         * Sends a JSON-based request to Pro Tools.
         * responseCallback provides an ability to process all respones (both intermediate and final) in realtime.
         * The callback is called from the service thread.
         */
        std::future<CppPTSLResponse> SendRequest(CppPTSLRequest request, std::function<void(const CppPTSLResponse&)> responseCallback = nullptr);

        /**
         * Cancels all requests that are currently in progress.
         */
        void CancelRequests(bool waitForCancel = true);

    public:
        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get task execution status
         *
         * Only returns the status of the task that is currently in progress. 
         * So it should be called asynchronously in parallel to the requested PTSL Command 
         * like it is implemented in @ref PTSLC_CPP::CppPTSLClient::PingTaskStatus.
         * 
         * @returns task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetTaskStatusResponse>
            GetTaskStatus(const GetTaskStatusRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Create a new Pro Tools session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CreateSession(const CreateSessionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Open a Pro Tools session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        OpenSession(const OpenSessionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Import existing Pro Tools session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<ImportResponse>
        Import(const ImportRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get list of tracks from session
         * 
         * @returns track list and task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetTrackListResponse>
            GetTrackList(const GetTrackListRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Select all clips on the selected track
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SelectAllClipsOnTrack(const SelectAllClipsOnTrackRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Extend selection to target tracks
         * 
         * @returns track list and task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ExtendSelectionToTargetTracks(const ExtendSelectionToTargetTracksRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
        * Trim clips to selection
         * 
         * @returns task execution status
         * 
        */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        TrimToSelection(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Create fades on clips from specified preset
         *
         * @deprecated CreateFade is deprecated starting in Pro Tools 2023.09.
         * Replaced with @ref PTSLC_CPP::CppPTSLClient::CreateFadesBasedOnPreset "CreateFadesBasedOnPreset"
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CreateFade(const CreateFadesBasedOnPresetRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Rename selected track
         *
         * @deprecated RenameTrack is deprecated starting in Pro Tools 2023.09.
         * Replaced with @ref PTSLC_CPP::CppPTSLClient::RenameTargetTrack "RenameTargetTrack"
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RenameTrack(const RenameTargetTrackRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Consolidate selected clips
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ConsolidateClip(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Export selected clips as files
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ExportClipsAsFiles(const ExportClipsAsFilesRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Export selected clips as AAF/OMF
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ExportSelectedTracksAsAAFOMF(const ExportSelectedTracksAsAAFOMFRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Force rescan the file list or force recalculate waveforms for the specific files
         * 
         * @returns task execution status and count of successfully and unsuccessfully scanned files
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<RefreshTargetAudioFilesResponse>
            RefreshTargetAudioFiles(const RefreshTargetAudioFilesRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Refresh modified files in the session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RefreshAllModifiedAudioFiles(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get an optionally filtered list of the media files referenced by the session.
         * 
         * @returns task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetFileLocationResponse>
            GetFileLocation(const GetFileLocationRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Close a Pro Tools session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CloseSession(const CloseSessionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Save a Pro Tools session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SaveSession(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Save Pro Tools session as a new session
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SaveSessionAs(const SaveSessionAsRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Cut data on track
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        Cut(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Copy data on track
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        Copy(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Paste data on track
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        Paste(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Clear data on track
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        Clear(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Cut data on track by filtering data type
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CutSpecial(const CutSpecialRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Copy data on track by filtering data type
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CopySpecial(const CopySpecialRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Clear data on track by filtering data type
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ClearSpecial(const ClearSpecialRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Pasting data on track by filtering data type
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        PasteSpecial(const PasteSpecialRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get edit mode.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetEditModeResponse>
            GetEditMode(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set edit mode.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetEditMode(const SetEditModeRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current edit tool.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetEditToolResponse>
            GetEditTool(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set edit tool.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetEditTool(const SetEditToolRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Recall zoom preset.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RecallZoomPreset(const RecallZoomPresetRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current edit mode options.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetEditModeOptionsResponse>
            GetEditModeOptions(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set edit mode options.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetEditModeOptions(const SetEditModeOptionsRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         @brief Render all or part of the session to one or more files

         @details Export Mix (Bounce Mix) can export an entire mix, including sub-mixes, to multiple file
         formats without having to create extra dedicated audio tracks in the session for bus recording.
         Different options and settings are available depending on the selected FileType. The options are
         organized in sections including Audio, Video, Location, and Dolby Atmos. Here's a breakdown of how the
         sections are used for each FileType:

         - WAV
            -# has no Video section
            -# has a unique field called DeliveryFormat (only shown if Interleaved is selected as the file format and there are multiple mix sources)

         - AIF
            -# has no Video section

         - MP3
            -# has no Video section
            -# does not allow multiple mix sources
            -# does not allow mix sources greater than 0
            -# does not allow specifying Bit Depth value (always a 16Bit)
            -# Sample Rate is limited to 48k and 44.1k
            -# Import after Bounce is not available

         - MXF OPAtom
            -# has no Video section
            -# File format is multi-mono
            -# Bit Depth is only 16Bit and 24Bit
            -# Sample Rate is limited to 48k and 44.1k
            -# Pad to frame boundary is hard-coded to ON

         - WAV (Dolby Atmos ADM BWF)
            -# has no Audio section
            -# has no Video section
            -# has Dolby Atmos section (unique to this format)
            -# has FFOA (bool) default to False
            -# if FFOA is true then there's an option to enter TimeCode value
                -# TimeCode value will match the session frame rate
                -# Frame rate will always be 23.976 and 30 non-drop (if Frame rate is 60 -> it will still be 30)
            -# has Embed Program-level Metadata

          @return Task execution status
          
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ExportMix(const ExportMixRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get dynamic properties from the host application
         * 
         * Use dynamic properties to query the application about its support for features such as codecs
         * that are not directly bound to a particular version of the API or host application.
         * 
         * @sa DynamicPropertyType
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetDynamicPropertiesResponse>
            GetDynamicProperties(const GetDynamicPropertiesRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Place clips at exact locations
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        Spot(const SpotRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Provide session data as text
         * 
         * @returns task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<ExportSessionInfoAsTextResponse>
            ExportSessionInfoAsText(const ExportSessionInfoAsTextRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set playback mode
         * 
         * @returns task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<SetPlaybackModeResponse>
            SetPlaybackMode(const SetPlaybackModeRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set record mode
         * 
         * @returns task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<SetRecordModeResponse>
            SetRecordMode(const SetRecordModeRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session audio files format
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionAudioFormatResponse>
            GetSessionAudioFormat(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session sample rate
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionSampleRateResponse>
            GetSessionSampleRate(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session bit depth
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionBitDepthResponse>
            GetSessionBitDepth(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session interleaved state
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionInterleavedStateResponse>
            GetSessionInterleavedState(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session time code rate
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionTimeCodeRateResponse>
            GetSessionTimeCodeRate(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session feet+frames rate
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionFeetFramesRateResponse>
            GetSessionFeetFramesRate(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session audio rate pull up/down
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionAudioRatePullSettingsResponse>
            GetSessionAudioRatePullSettings(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session video rate pull up/down
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionVideoRatePullSettingsResponse>
            GetSessionVideoRatePullSettings(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session name
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionNameResponse>
            GetSessionName(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session path
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionPathResponse>
            GetSessionPath(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session start time
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionStartTimeResponse>
            GetSessionStartTime(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current session length
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionLengthResponse>
            GetSessionLength(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session audio files format
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionAudioFormat(const SetSessionAudioFormatRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session bit depth
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionBitDepth(const SetSessionBitDepthRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session interleaved state
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionInterleavedState(const SetSessionInterleavedStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session time code rate
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionTimeCodeRate(const SetSessionTimeCodeRateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session feet+frames rate
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionFeetFramesRate(const SetSessionFeetFramesRateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session audio rate pull up/down
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionAudioRatePullSettings(const SetSessionAudioRatePullSettingsRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session video rate pull up/down
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionVideoRatePullSettings(const SetSessionVideoRatePullSettingsRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session start time
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionStartTime(const SetSessionStartTimeRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set current session length
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSessionLength(const SetSessionLengthRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current version of PTSL Host, Client or Client's API Methods
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetPTSLVersionResponse>
            GetPTSLVersion(const GetPTSLVersionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current playback mode
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetPlaybackModeResponse>
            GetPlaybackMode(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current record mode
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetRecordModeResponse>
            GetRecordMode(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get transport armed
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetTransportArmedResponse>
            GetTransportArmed(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get current transport state
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetTransportStateResponse>
            GetTransportState(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Removes memory locations corresponding to the numbers provided in the list. 
         * Memory location enumeration should start with 1 (using 0 or negative numbers may result in undefined behavior).
         * 
         * @returns task execution status and count of successfully and unsuccessfully cleared memory locations
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<ClearMemoryLocationResponse>
            ClearMemoryLocation(const ClearMemoryLocationRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Renames a clip that was selected in the Pro Tools.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RenameSelectedClip(const RenameSelectedClipRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Renames a clip by its name.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RenameTargetClip(const RenameTargetClipRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Toggles current play state (behaves the same as the button press)
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        TogglePlayState(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Toggles current record enable (behaves the same as the button press)
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ToggleRecordEnable(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Plays with half speed
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        PlayHalfSpeed(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Records with half speed
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RecordHalfSpeed(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Edit memory location.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        EditMemoryLocation(const EditMemoryLocationRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Create memory location.
         *
        */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CreateMemoryLocation(const CreateMemoryLocationRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get memory locations.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetMemoryLocationsResponse>
            GetMemoryLocations(const GetMemoryLocationsRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Submit registration information to create a new connection
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<RegisterConnectionResponse>
            RegisterConnection(const RegisterConnectionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Create fades on clips from specified preset
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        CreateFadesBasedOnPreset(const CreateFadesBasedOnPresetRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Rename selected track
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RenameTargetTrack(const RenameTargetTrackRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Create new tracks
         *
         * @returns task execution status
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<CreateNewTracksResponse>
            CreateNewTracks(const CreateNewTracksRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set timeline selection
         *
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTimelineSelection(const SetTimelineSelectionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Get timeline selection.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetTimelineSelectionResponse>
            GetTimelineSelection(const GetTimelineSelectionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Select tracks by specified names without using wildcards.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<SelectTracksByNameResponse>
            SelectTracksByName(const SelectTracksByNameRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Import Video file(s) into the session
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<ImportVideoResponse>
            ImportVideo(const ImportVideoRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Select the memory location specified
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SelectMemoryLocation(const SelectMemoryLocationRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `mute` state of the specified tracks (except Video and MasterFader types of tracks)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackMuteState(const SetTrackMuteStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `solo` state of the specified tracks (except Video and MasterFader types of tracks)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackSoloState(const SetTrackSoloStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `solo safe` state of the specified tracks (except Video and MasterFader types of tracks)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackSoloSafeState(const SetTrackSoloSafeStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `record enable` state of the specified tracks (Audio, Instrument, Midi and VCA types of tracks only)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackRecordEnableState(const SetTrackRecordEnableStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `record safe enable` state of the specified tracks (Audio, Instrument, Midi and VCA types of tracks only)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackRecordSafeEnableState(const SetTrackRecordSafeEnableStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `input monitor` state of the specified tracks (Audio and VCA types of tracks only)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackInputMonitorState(const SetTrackInputMonitorStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `smart DSP` state of the specified tracks (except Video, Midi, VCA and Basic Folder types of tracks)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackSmartDspState(const SetTrackSmartDspStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `hidden` state of the specified tracks
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackHiddenState(const SetTrackHiddenStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `inactive` state of the specified tracks (except Video tracks)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackInactiveState(const SetTrackInactiveStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `frozen` state of the specified tracks (Audio, Instrument, AuxInput and Routing Folder types of tracks only)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackFrozenState(const SetTrackFrozenStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `online` state of the specified tracks (Video tracks only)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackOnlineState(const SetTrackOnlineStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Set the `open` state of the specified tracks (Folder tracks only)
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackOpenState(const SetTrackOpenStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Returns originId, instanceId and parentId of the current opened session:
         * - originId is the main ID of the session (or project), and it remains the same in all variations of the session through Save As, Save Copy In, etc.
         * - instanceId is unique for each session variation. In a new/original session file, the instanceId is equal to originId. When variants are introduced via Save As, Save Copy In, and several others, instanceId is set to a new unique ID.
         * - parentId is set to the instanceId of the source session for sessions based off of a previous session. For new/original sessions, the parentId is set to zeros. That means parentId is equal to originId for first-generation variants, and is different for second and later generation variations.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionIDsResponse>
            GetSessionIDs(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Returns the Memory Locations Manage Mode state (see menu Window -> Memory Locations in the UI).
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetMemoryLocationsManageModeResponse>
            GetMemoryLocationsManageMode(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Sets the Memory Locations Manage Mode (see menu Window -> Memory Locations in the UI).
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetMemoryLocationsManageMode(const SetMemoryLocationsManageModeRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Sets the time format of the Main Counter.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetMainCounterFormat(const SetMainCounterFormatRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Sets the time format of the Sub Counter.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetSubCounterFormat(const SetSubCounterFormatRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Returns the time format of the Main Counter.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetMainCounterFormatResponse>
            GetMainCounterFormat(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Returns the time format of the Sub Counter.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSubCounterFormatResponse>
            GetSubCounterFormat(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Undoes the last N operations. Successfully undone operations are returned in the response.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<UndoResponse>
        Undo(const UndoRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Redoes the last N operations. Successfully redone operations are returned in the response.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<RedoResponse>
        Redo(const RedoRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Undoes all operations listed in Undo History.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<UndoAllResponse>
        UndoAll(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Redoes all operations listed in Undo History.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<RedoAllResponse>
        RedoAll(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Clears the undo queue.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        ClearUndoQueue(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Sets the DSP Mode Safe state for the specified tracks.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        SetTrackDSPModeSafeState(const SetTrackDSPModeSafeStateRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Provides the System Delay value and the Delay Compensation state of the current session.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<GetSessionSystemDelayInfoResponse>
            GetSessionSystemDelayInfo(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Creates a new clip group based on the Edit selection made across tracks.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        GroupClips(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Reveals all underlying clips and any nested clip groups within a selected clip group, making them independent and editable. Note: Only the front-most top-level clip group will be ungrouped; any underlying clip groups will be preserved.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        UngroupClips(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Reveals all clips within a selected clip group and any of its nested clip groups, making them independent and editable.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        UngroupAllClips(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Reassembles a previously ungrouped clip group, including any nested clip groups, restoring its original structure.
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RegroupClips(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Checks to see if the Pro Tools application is fully loaded and ready to execute PTSL commands.
         * Called by internal client wrapper utility function @ref PTSLC_CPP::CppPTSLClient::HostReadyCheck.
         * 
         * @returns the status of the PTSL host
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<HostReadyCheckResponse>
            HostReadyCheck(const CommandRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Lets you specify the number of times the selected material is duplicated.
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        RepeatSelection(const RepeatSelectionRequest& request);

        /**
         * @deprecated All the API-specific functions (commands) are deprecated starting in Pro Tools 2024.10.
         * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function instead.
         *
         * Copies a selection and places it immediately after the end of the selection.
         * 
         * @returns task execution status
         */
        [[deprecated(
            "All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
            "Please use a general JSON-based SendRequest function instead.")]] virtual std::shared_ptr<CommandResponse>
        DuplicateSelection(const CommandRequest& request);

        /**
         * Clears all the memory locations in the session.
         */
        [[deprecated("All API-specific functions (commands) are deprecated starting in Pro Tools 2024.10. "
                     "Please use a general JSON-based SendRequest function instead.")]] virtual std::
            shared_ptr<ClearAllMemoryLocationsResponse>
            ClearAllMemoryLocations(const CommandRequest& request);

    protected:
        /**
         * Used for initialization client, starting PT and host (if required) and checking if host ready.
         */
        void Init();

        /**
         * Used to free up client resources.
         */
        void Free();

        /**
        * Internal client wrapper utility function called by the constructor, @ref PTSLC_CPP::CppPTSLClient::CppPTSLClient
        * and checks to see if the Pro Tools application is ready to execute PTSL commands.
        */
        void HostReadyCheck();

    private:
        struct InternalData;

    private:
        /**
         * A client's session identifier.
         * As a result of @ref RegisterConnection, this value is returned.
         * With every subsequent command, this value is then passed.
         */
        std::string SessionId;

        /**
         * Flag which prevents commands execution if Host is not ready.
         */
        bool m_isHostReady;

        /**
         * Settings which are used for client initialization.
         */
        ClientConfig m_clientConfig;

        /**
         *  Wrapper for internal library dependent data.
         */
        std::unique_ptr<InternalData> m_internalData;

        /**
         *  Used for synchronized access to the session id.
         */
        mutable std::mutex SessionIdMutex;

    private:
        /**
         * Starts the server. Use this method only for unit tests.
         * In real use, PTSL.dll will load together during Pro Tools launch.
         * If you want to debug the server via client and launch server separately,
         * you need to comment calls of this method in the client constructor.
         */
        bool LocateServer();

        /**
         * Internal client wrapper feature.
         * Periodically checks the status of command execution with PING_TIMEOUT timeout.
         * Called right after gRPC request sent to the server and terminate command execution
         * if status of GetTaskStatus command execution is 'Failed'.
         */
        void PingTaskStatus(std::string responseReceivedTaskId, std::future<void> pingCancellationToken);

        /**
         * Fills error structure and sends to caller in case the server is not available.
         */
        [[deprecated("Deprecated starting in Pro Tools 2024.10. "
                     "Please use SendHostNotReadyResponse for non-legacy code.")]] std::shared_ptr<CommandResponse>
        SendErrorResponse(CommandId commandType);

        /**
         * Internal wrapper function used to alert the client app that an error occurred prior to sending the command to the host app.
         */
        CppPTSLResponse SendErrorResponse(
            CommandId commandType, CommandErrorType errorType, const std::string& errorMessage = "");

        /**
         * Internal wrapper function used to alert the client app that the host app is not ready to receive commands.
         */
        CppPTSLResponse SendHostNotReadyResponse(CommandId commandType);

        std::string LookForSessionId(const std::string& responseBodyJson);
    };
} // namespace PTSLC_CPP
