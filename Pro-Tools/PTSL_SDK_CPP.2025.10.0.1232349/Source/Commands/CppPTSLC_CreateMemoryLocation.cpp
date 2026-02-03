// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CreateMemoryLocation command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::CreateMemoryLocation(const CreateMemoryLocationRequest& request)
    {
        struct CreateMemoryLocationHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CreateMemoryLocation);

            std::string GetRequestName() const override
            {
                return "AsyncCreateMemoryLocation";
            }

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
            void FillGrpcRequest(const CreateMemoryLocationRequest& request)
            {
                mGrpcRequestBody.set_number(static_cast<int32_t>(request.number));
                mGrpcRequestBody.set_name(request.name);
                mGrpcRequestBody.set_start_time(request.startTime);
                mGrpcRequestBody.set_end_time(request.endTime);
                mGrpcRequestBody.set_time_properties(static_cast<ptsl::TimeProperties>(request.timeProperties));
                mGrpcRequestBody.set_reference(static_cast<ptsl::MemoryLocationReference>(request.reference));
                mGrpcRequestBody.set_comments(request.comments);

                mGrpcRequestBody.mutable_general_properties()->set_zoom_settings(
                    request.generalProperties.zoomSettings);
                mGrpcRequestBody.mutable_general_properties()->set_pre_post_roll_times(
                    request.generalProperties.prePostRollTimes);
                mGrpcRequestBody.mutable_general_properties()->set_track_visibility(
                    request.generalProperties.trackVisibility);
                mGrpcRequestBody.mutable_general_properties()->set_track_heights(
                    request.generalProperties.trackHeights);
                mGrpcRequestBody.mutable_general_properties()->set_group_enables(
                    request.generalProperties.groupEnables);
                mGrpcRequestBody.mutable_general_properties()->set_window_configuration(
                    request.generalProperties.windowConfiguration);
                mGrpcRequestBody.mutable_general_properties()->set_window_configuration_index(
                    request.generalProperties.windowConfigurationIndex);
                mGrpcRequestBody.mutable_general_properties()->set_venue_snapshot_index(
                    request.generalProperties.venueSnapshotIndex);
                mGrpcRequestBody.mutable_general_properties()->set_venue_snapshot_name(
                    request.generalProperties.venueSnapshotName);
                mGrpcRequestBody.set_color_index(static_cast<int32_t>(request.colorIndex));
                mGrpcRequestBody.set_location(static_cast<ptsl::MarkerLocation>(request.location));
                mGrpcRequestBody.set_track_name(request.trackName);
            }

            ptsl::CreateMemoryLocationRequestBody mGrpcRequestBody;
            ptsl::CreateMemoryLocationResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(CreateMemoryLocation, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
