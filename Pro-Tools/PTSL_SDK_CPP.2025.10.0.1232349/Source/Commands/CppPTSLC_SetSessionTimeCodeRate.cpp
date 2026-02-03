// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionTimeCodeRate command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionTimeCodeRate(const SetSessionTimeCodeRateRequest& request)
    {
        struct SetSessionTimeCodeRateHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionTimeCodeRate);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionTimeCodeRate";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionTimeCodeRateRequest& request)
            {
                mGrpcRequestBody.set_time_code_rate(static_cast<ptsl::SessionTimeCodeRate>(request.timeCodeRate));
            }

            ptsl::SetSessionTimeCodeRateRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionTimeCodeRate, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
