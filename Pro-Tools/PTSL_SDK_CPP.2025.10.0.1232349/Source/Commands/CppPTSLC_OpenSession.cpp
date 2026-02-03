// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::OpenSession command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::OpenSession(const OpenSessionRequest& request)
    {
        struct OpenSessionHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(OpenSession);

            std::string GetRequestName() const override
            {
                return "AsyncOpenSession";
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
            void FillGrpcRequest(const OpenSessionRequest& request)
            {
                mGrpcRequestBody.set_session_path(request.sessionPath);
            }

            ptsl::OpenSessionRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(OpenSession, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
