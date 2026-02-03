// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SetSessionLength command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SetSessionLength(const SetSessionLengthRequest& request)
    {
        struct SetSessionLengthHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(SetSessionLength);

            std::string GetRequestName() const override
            {
                return "AsyncSetSessionLength";
            }

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

        private:
            void FillGrpcRequest(const SetSessionLengthRequest& request)
            {
                mGrpcRequestBody.set_session_length(request.sessionLength);
            }

            ptsl::SetSessionLengthRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(SetSessionLength, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
