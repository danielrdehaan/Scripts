// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionStartTime command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionStartTimeResponse> CppPTSLClient::GetSessionStartTime(const CommandRequest& request)
    {
        struct GetSessionStartTimeHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionStartTime);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionStartTime";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionStartTimeResponse>(mResponse)->sessionStartTime =
                    static_cast<std::string>(mGrpcResponseBody.session_start_time());
            }

            MAKE_RESP_OVRD(GetSessionStartTime);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionStartTimeResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionStartTime, false, true);

        return std::dynamic_pointer_cast<GetSessionStartTimeResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
