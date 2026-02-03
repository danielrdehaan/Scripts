// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionSystemDelayInfo command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionSystemDelayInfoResponse> CppPTSLClient::GetSessionSystemDelayInfo(
        const CommandRequest& request)
    {
        struct GetSessionSystemDelayInfoHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionSystemDelayInfo);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionSystemDelayInfo";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionSystemDelayInfoResponse>(mResponse)->samples =
                    mGrpcResponseBody.samples();
                std::dynamic_pointer_cast<GetSessionSystemDelayInfoResponse>(mResponse)->delayCompensationEnabled =
                    mGrpcResponseBody.delay_compensation_enabled();
            }

            MAKE_RESP_OVRD(GetSessionSystemDelayInfo);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionSystemDelayInfoResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionSystemDelayInfo, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<GetSessionSystemDelayInfoResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
