// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetMemoryLocations command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetMemoryLocationsResponse> CppPTSLClient::GetMemoryLocations(
        const GetMemoryLocationsRequest& request)
    {
        struct GetMemoryLocationsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(GetMemoryLocations);

            std::string GetRequestName() const override
            {
                return "AsyncGetMemoryLocations";
            }

            void OnHasBody() override
            {
                std::vector<MemoryLocation> memoryLocationsTempList;
                for (const auto& item : mGrpcResponseBody.memory_locations())
                {
                    MemoryLocation memoryLocation;

                    memoryLocation.number = item.number();
                    memoryLocation.name = item.name();
                    memoryLocation.startTime = item.start_time();
                    memoryLocation.endTime = item.end_time();
                    memoryLocation.timeProperties = static_cast<TimeProperties>(item.time_properties());
                    memoryLocation.reference = static_cast<MemoryLocationReference>(item.reference());
                    memoryLocation.comments = item.comments();
                    memoryLocation.location = static_cast<MarkerLocation>(item.location());
                    memoryLocation.trackName = item.track_name();
                    memoryLocation.colorIndex = item.color_index();

                    MemoryLocationProperties generalProperties;
                    generalProperties.zoomSettings = item.general_properties().zoom_settings();
                    generalProperties.prePostRollTimes = item.general_properties().pre_post_roll_times();
                    generalProperties.trackVisibility = item.general_properties().track_visibility();
                    generalProperties.trackHeights = item.general_properties().track_heights();
                    generalProperties.groupEnables = item.general_properties().group_enables();
                    generalProperties.windowConfiguration = item.general_properties().window_configuration();
                    generalProperties.windowConfigurationIndex = item.general_properties().window_configuration_index();
                    generalProperties.windowConfigurationName = item.general_properties().window_configuration_name();
                    generalProperties.venueSnapshotIndex = item.general_properties().venue_snapshot_index();
                    generalProperties.venueSnapshotName = item.general_properties().venue_snapshot_name();
                    memoryLocation.generalProperties = generalProperties;

                    memoryLocationsTempList.push_back(memoryLocation);
                }

                std::dynamic_pointer_cast<GetMemoryLocationsResponse>(mResponse)->memoryLocations = {
                    memoryLocationsTempList.begin(), memoryLocationsTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<GetMemoryLocationsResponse>(mResponse)->memoryLocations.resize(0);
            }

            MAKE_RESP_OVRD(GetMemoryLocations);

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
            void FillGrpcRequest(const GetMemoryLocationsRequest& request)
            {
                mGrpcRequestBody.mutable_pagination_request()->set_limit(request.paginationRequest.limit);
                mGrpcRequestBody.mutable_pagination_request()->set_offset(request.paginationRequest.offset);
            }

            ptsl::GetMemoryLocationsRequestBody mGrpcRequestBody;
            ptsl::GetMemoryLocationsResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetMemoryLocations, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);

        return std::dynamic_pointer_cast<GetMemoryLocationsResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
