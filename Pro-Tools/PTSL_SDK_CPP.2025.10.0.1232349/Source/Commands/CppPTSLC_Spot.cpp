// Copyright 2021-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::Spot command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::Spot(const SpotRequest& request)
    {
        struct SpotHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(Spot);

            std::string GetRequestName() const override
            {
                return "AsyncSpot";
            }

            bool IsNeedToPingTaskStatus() const override
            {
                return true;
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SpotRequest& request)
            {
                // deprecated starting in Pro Tools 2023.12
                mGrpcRequestBody.mutable_location_data()->set_location_options(
                    static_cast<ptsl::TrackOffsetOptions>(request.locationOptions));
                // deprecated starting in Pro Tools 2023.12
                mGrpcRequestBody.mutable_location_data()->set_location_type(
                    static_cast<ptsl::SpotLocationType>(request.locationType));
                // deprecated starting in Pro Tools 2023.12
                mGrpcRequestBody.mutable_location_data()->set_location_value(request.locationValue);

                mGrpcRequestBody.mutable_location_data()->set_location_options(
                    static_cast<ptsl::TrackOffsetOptions>(request.spotLocationData.locationOptions));
                mGrpcRequestBody.mutable_location_data()->set_location_type(
                    static_cast<ptsl::SpotLocationType>(request.spotLocationData.locationType));
                mGrpcRequestBody.mutable_location_data()->set_location_value(request.spotLocationData.locationValue);
            }

            ptsl::SpotRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(Spot, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
