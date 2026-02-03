// Copyright 2021-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::Import command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<ImportResponse> CppPTSLClient::Import(const ImportRequest& request)
    {
        struct ImportHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(Import);

            std::string GetRequestName() const override
            {
                return "AsyncImport";
            }

            void OnHasBody() override
            {
                for (const auto& item : mGrpcResponseBody.mutable_audio_data()->file_list())
                {
                    std::dynamic_pointer_cast<ImportResponse>(mResponse)->audioData.filesList.push_back(item);
                }

                std::dynamic_pointer_cast<ImportResponse>(mResponse)->audioData.audioOperations =
                    static_cast<AudioOperations>(mGrpcResponseBody.mutable_audio_data()->audio_operations());
                std::dynamic_pointer_cast<ImportResponse>(mResponse)->audioData.destinationPath =
                    mGrpcResponseBody.mutable_audio_data()->destination_path();
                std::dynamic_pointer_cast<ImportResponse>(mResponse)->audioData.destination =
                    static_cast<MediaDestination>(mGrpcResponseBody.mutable_audio_data()->audio_destination());
                std::dynamic_pointer_cast<ImportResponse>(mResponse)->audioData.location =
                    static_cast<MediaLocation>(mGrpcResponseBody.mutable_audio_data()->audio_location());
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<ImportResponse>(mResponse)->audioData.filesList.resize(0);
            }

            bool IsNeedToPingTaskStatus() const override
            {
                return true;
            }

            MAKE_RESP_OVRD(Import);

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            void FillGrpcRequest(const ImportRequest& request)
            {
                mGrpcRequestBody.set_session_path(request.sessionPath);
                mGrpcRequestBody.set_import_type(static_cast<ptsl::ImportType>(request.importType));
                mGrpcRequestBody.mutable_session_data()->set_audio_options(
                    static_cast<ptsl::AudioMediaOptions>(request.audioOptions));
                mGrpcRequestBody.mutable_session_data()->set_audio_handle_size(request.audioHandleSize);
                mGrpcRequestBody.mutable_session_data()->set_video_options(
                    static_cast<ptsl::VideoMediaOptions>(request.videoOptions));
                mGrpcRequestBody.mutable_session_data()->set_match_options(
                    static_cast<ptsl::MatchTrackOptions>(request.matchOptions));
                mGrpcRequestBody.mutable_session_data()->set_playlist_options(
                    static_cast<ptsl::MainPlaylistOptions>(request.playlistOptions));
                mGrpcRequestBody.mutable_session_data()->mutable_track_data_to_import()->set_track_data_preset_path(
                    request.trackDataPresetPath);
                mGrpcRequestBody.mutable_session_data()->mutable_track_data_to_import()->set_clip_gain(
                    request.clipGain);
                mGrpcRequestBody.mutable_session_data()->mutable_track_data_to_import()->set_clips_and_media(
                    request.clipsAndMedia);
                mGrpcRequestBody.mutable_session_data()->mutable_track_data_to_import()->set_volume_automation(
                    request.volumeAutomation);
                mGrpcRequestBody.mutable_session_data()->set_timecode_mapping_units(
                    static_cast<ptsl::TimeCodeMappingOptions>(request.timeCodeMappingUnits));
                mGrpcRequestBody.mutable_session_data()->set_timecode_mapping_start_time(
                    request.timeCodeMappingStartTime);
                mGrpcRequestBody.mutable_session_data()->set_adjust_session_start_time_to_match_source(
                    request.adjustSessionStartTimeToMatchSource);

                if ((ptsl::ImportType)request.importType == ptsl::ImportType::IType_Audio)
                {
                    *mGrpcRequestBody.mutable_audio_data()->mutable_file_list() = { request.audioData.filesList.begin(),
                        request.audioData.filesList.end() };
                    mGrpcRequestBody.mutable_audio_data()->set_audio_operations(
                        static_cast<ptsl::AudioOperations>(request.audioData.audioOperations));
                    mGrpcRequestBody.mutable_audio_data()->set_destination_path(request.audioData.destinationPath);
                    mGrpcRequestBody.mutable_audio_data()->set_audio_destination(
                        static_cast<ptsl::MediaDestination>(request.audioData.destination));
                    mGrpcRequestBody.mutable_audio_data()->set_audio_location(
                        static_cast<ptsl::MediaLocation>(request.audioData.location));
                    mGrpcRequestBody.mutable_audio_data()->mutable_location_data()->set_location_type(
                        static_cast<ptsl::SpotLocationType>(request.locationType));
                    mGrpcRequestBody.mutable_audio_data()->mutable_location_data()->set_location_value(
                        request.locationValue);
                    mGrpcRequestBody.mutable_audio_data()->mutable_location_data()->set_location_options(
                        static_cast<ptsl::TrackOffsetOptions>(request.locationOptions));
                }
            }

            ptsl::ImportRequestBody mGrpcRequestBody;
            ptsl::ImportResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(Import, true, true);

        return std::dynamic_pointer_cast<ImportResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
