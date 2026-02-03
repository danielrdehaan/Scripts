// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionVideoRatePullSettings command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionVideoRatePullSettings(
        const SetSessionVideoRatePullSettingsRequest& request)
    {
        struct SetSessionVideoRatePullSettingsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionVideoRatePullSettings);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionVideoRatePullSettings";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionVideoRatePullSettingsRequest& request)
            {
                mGrpcRequestBody.set_video_rate_pull(static_cast<ptsl::SessionRatePull>(request.videoRatePull));
            }

            ptsl::SetSessionVideoRatePullSettingsRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionVideoRatePullSettings, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
