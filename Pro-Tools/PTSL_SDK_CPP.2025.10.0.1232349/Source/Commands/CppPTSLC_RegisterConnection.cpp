// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information
// of Avid. Do not disclose to any third party. Use of the
// information contained in this document is subject to an Avid
// SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RegisterConnection command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<RegisterConnectionResponse> CppPTSLClient::RegisterConnection(
        const RegisterConnectionRequest& request)
    {
        struct RegisterConnectionHandler : public DefaultRequestHandler
        {
        public:
            RegisterConnectionHandler()
                : mClient(nullptr)
            {

            }
            
            RegisterConnectionHandler(CppPTSLClient* client, const RegisterConnectionRequest& request)
                : mClient(client)
            {
                std::string jsonBody = request.directJsonBody;
                if (!jsonBody.empty() && !std::all_of(jsonBody.begin(), jsonBody.end(), isspace))
                {
                    google::protobuf::util::JsonStringToMessage(jsonBody, &mGrpcRequestBody);
                }
                else
                {
                    this->FillGrpcRequest(request);
                }
            }

            std::string GetRequestName() const override
            {
                return "AsyncRegisterConnection";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<RegisterConnectionResponse>(mResponse)->sessionId =
                    static_cast<std::string>(mGrpcResponseBody.session_id());
                mClient->SetSessionId(static_cast<std::string>(mGrpcResponseBody.session_id()));
            }

            void OnNoBody() override
            {
            }

            MAKE_RESP_OVRD(RegisterConnection);

        protected:
            google::protobuf::Message& GetRequestBodyRef() override
            {
                return mGrpcRequestBody;
            }

            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::RegisterConnectionRequestBody mGrpcRequestBody;
            ptsl::RegisterConnectionResponseBody mGrpcResponseBody;
            CppPTSLClient* mClient;

            void FillGrpcRequest(const RegisterConnectionRequest& request)
            {
                mGrpcRequestBody.set_company_name(std::string(request.companyName));
                mGrpcRequestBody.set_application_name(std::string(request.applicationName));
            }
        };
        
        std::shared_ptr<DefaultRequestHandler> hndlr = std::make_shared<RegisterConnectionHandler>(this, request);
        this->MakeStreamingRequest(hndlr, CommandId::CId_RegisterConnection);
        return std::dynamic_pointer_cast<RegisterConnectionResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
