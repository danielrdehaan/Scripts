// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetSessionLength command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetSessionLengthResponse> CppPTSLClient::GetSessionLength(const CommandRequest& request)
    {
        struct GetSessionLengthHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GetSessionLength);

            std::string GetRequestName() const override
            {
                return "AsyncGetSessionLength";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetSessionLengthResponse>(mResponse)->sessionLength =
                    static_cast<std::string>(mGrpcResponseBody.session_length());
            }

            MAKE_RESP_OVRD(GetSessionLength);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::GetSessionLengthResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetSessionLength, false, true);

        return std::dynamic_pointer_cast<GetSessionLengthResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
