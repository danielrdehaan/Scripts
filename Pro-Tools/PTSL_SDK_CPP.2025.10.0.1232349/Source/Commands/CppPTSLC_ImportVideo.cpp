// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ImportVideo command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<ImportVideoResponse> CppPTSLClient::ImportVideo(const ImportVideoRequest& request)
    {
        struct ImportVideoHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ImportVideo);

            std::string GetRequestName() const override
            {
                return "AsyncImportVideo";
            }

            void OnHasBody() override
            {
                std::vector<ImportFailureInfo> failureListTemp;

                for (const auto& item : mGrpcResponseBody.failure_list())
                {
                    ImportFailureInfo failureInfo { item.file_path(), item.failure_message() };
                    failureListTemp.push_back(failureInfo);
                }

                std::dynamic_pointer_cast<ImportVideoResponse>(mResponse)->failureList = { failureListTemp.begin(),
                    failureListTemp.end() };
            }

            MAKE_RESP_OVRD(ImportVideo);

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
            void FillGrpcRequest(const ImportVideoRequest& request)
            {
                *mGrpcRequestBody.mutable_video_file_list() = { request.videoFilesList.begin(),
                    request.videoFilesList.end() };
                mGrpcRequestBody.set_destination(static_cast<ptsl::MediaDestination>(request.destination));
                mGrpcRequestBody.set_location(static_cast<ptsl::MediaLocation>(request.location));
                mGrpcRequestBody.mutable_spot_location_data()->set_location_type(
                    static_cast<ptsl::SpotLocationType>(request.spotLocationData.locationType));
                mGrpcRequestBody.mutable_spot_location_data()->set_location_value(
                    request.spotLocationData.locationValue);
                mGrpcRequestBody.mutable_spot_location_data()->set_location_options(
                    static_cast<ptsl::TrackOffsetOptions>(request.spotLocationData.locationOptions));
                mGrpcRequestBody.set_gaps_between_clips(request.gapsBetweenClips);
                mGrpcRequestBody.set_import_audio_from_file(request.importAudioFromFile);
                mGrpcRequestBody.set_audio_destination_path(request.audioDestinationPath);
                mGrpcRequestBody.set_remove_existing_video_tracks(request.removeExistingVideoTracks);
                mGrpcRequestBody.set_remove_existing_video_clips(request.removeExistingVideoClips);
                mGrpcRequestBody.set_clear_destination_video_track_playlist(request.clearDestinationVideoTrackPlaylist);
            }

            ptsl::ImportVideoRequestBody mGrpcRequestBody;
            ptsl::ImportVideoResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(ImportVideo, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);

        return std::dynamic_pointer_cast<ImportVideoResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
