// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::CloseSession command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::CloseSession(const CloseSessionRequest& request)
    {
        struct CloseSessionHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(CloseSession);

            std::string GetRequestName() const override
            {
                return "AsyncCloseSession";
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
            void FillGrpcRequest(const CloseSessionRequest& request)
            {
                mGrpcRequestBody.set_save_on_close(request.saveOnClose);
            }

            ptsl::CloseSessionRequestBody mGrpcRequestBody;
        };

        MAKE_REQUEST(CloseSession, true, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
