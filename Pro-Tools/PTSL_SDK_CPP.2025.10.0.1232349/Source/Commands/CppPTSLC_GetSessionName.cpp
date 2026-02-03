// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionName command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionNameResponse> CppPTSLClient::GetSessionName(const CommandRequest& request)
    {
        struct GetSessionNameHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionName);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionName";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionNameResponse>(mResponse)->sessionName =
                    static_cast<std::string>(mGrpcResponseBody.session_name());
            }

            MAKE_RESP_OVRD(GetSessionName);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionNameResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionName, false, true);

        return std::dynamic_pointer_cast<GetSessionNameResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
