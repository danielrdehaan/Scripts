// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CreateNewTracks command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CreateNewTracksResponse> CppPTSLClient::CreateNewTracks(const CreateNewTracksRequest& request)
    {
        struct CreateNewTracksHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CreateNewTracks);

            std::string GetRequestName() const override
            {
                return "AsyncCreateNewTracks";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<CreateNewTracksResponse>(mResponse)->numberOfTracks =
                    mGrpcResponseBody.number_of_tracks();
                std::dynamic_pointer_cast<CreateNewTracksResponse>(mResponse)->createdTrackNames = {
                    mGrpcResponseBody.created_track_names().begin(), mGrpcResponseBody.created_track_names().end()
                };
            }

            bool IsNeedToPingTaskStatus() const override
            {
                return true;
            }

            MAKE_RESP_OVRD(CreateNewTracks);

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
            void FillGrpcRequest(const CreateNewTracksRequest& request)
            {
                mGrpcRequestBody.mutable_pagination_request()->set_limit(request.paginationRequest.limit);
                mGrpcRequestBody.mutable_pagination_request()->set_offset(request.paginationRequest.offset);

                mGrpcRequestBody.set_number_of_tracks(request.numberOfTracks);
                mGrpcRequestBody.set_track_name(request.trackName);
                mGrpcRequestBody.set_track_format(static_cast<ptsl::TrackFormat>(request.trackFormat));
                mGrpcRequestBody.set_track_type(static_cast<ptsl::TrackType>(request.trackType));
                mGrpcRequestBody.set_track_timebase(static_cast<ptsl::TrackTimebase>(request.trackTimebase));
                mGrpcRequestBody.set_insertion_point_position(
                    static_cast<ptsl::TrackInsertionPoint>(request.insertionPointPosition));
                mGrpcRequestBody.set_insertion_point_track_name(request.insertionPointTrackName);
            }

            ptsl::CreateNewTracksRequestBody mGrpcRequestBody;
            ptsl::CreateNewTracksResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(CreateNewTracks, true, true);

        return std::dynamic_pointer_cast<CreateNewTracksResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
