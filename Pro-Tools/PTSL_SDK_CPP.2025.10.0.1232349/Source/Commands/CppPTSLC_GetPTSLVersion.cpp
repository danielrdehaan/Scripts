// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetPTSLVersion command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetPTSLVersionResponse> CppPTSLClient::GetPTSLVersion(const GetPTSLVersionRequest& request)
    {
        if (request.ptslComponent == VersionedPTSLComponents::VPTSLComponents_Host)
        {
            // Get PTSL Host version from Server side
            struct GetPTSLVersionHandler : public DefaultRequestHandler
            {
            public:
                INIT_HNDLR(GetPTSLVersion);

                std::string GetRequestName() const override
                {
                    return "AsyncGetPTSLVersion";
                }

                void OnHasBody() override
                {
                    std::dynamic_pointer_cast<GetPTSLVersionResponse>(mResponse)->ptslHostVersion =
                        mGrpcResponseBody.version();
                    std::dynamic_pointer_cast<GetPTSLVersionResponse>(mResponse)->apiMethodsVersionsList.clear();
                }

                void OnNoBody() override
                {
                    std::dynamic_pointer_cast<GetPTSLVersionResponse>(mResponse)->apiMethodsVersionsList.clear();
                }

                MAKE_RESP_OVRD(GetPTSLVersion);

            protected:
                google::protobuf::Message& GetResponseBodyRef() override
                {
                    return mGrpcResponseBody;
                }

            private:
                ptsl::GetPTSLVersionResponseBody mGrpcResponseBody;
            };

            MAKE_REQUEST(GetPTSLVersion, false, true);

            return std::dynamic_pointer_cast<GetPTSLVersionResponse>(hndlr->GetResponse());
        }
        else
        {
            auto response = std::make_shared<GetPTSLVersionResponse>();

            response->header.commandType = CommandId::CId_GetPTSLVersion;
            response->header.taskId = "empty";
            response->status.progress = 100;
            response->status.type = TaskStatus::TStatus_Completed;
            response->errors.clear();

            if (request.ptslComponent == VersionedPTSLComponents::VPTSLComponents_Client)
            {
                response->apiMethodsVersionsList["Client"] = PTSL_VERSION_MAJOR; // Client version
            }
            else if (request.ptslComponent == VersionedPTSLComponents::VPTSLComponents_ClientAPI)
            {
                response->apiMethodsVersionsList = PTSL_CLIENT_API_VERSIONS; // Client API versions
            }

            return response;
        }
    }
}; // namespace PTSLC_CPP
