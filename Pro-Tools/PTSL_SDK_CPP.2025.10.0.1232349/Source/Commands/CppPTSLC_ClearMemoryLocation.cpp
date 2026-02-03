// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ClearMemoryLocation command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<ClearMemoryLocationResponse> CppPTSLClient::ClearMemoryLocation(
        const ClearMemoryLocationRequest& request)
    {
        struct ClearMemoryLocationHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(ClearMemoryLocation);

            std::string GetRequestName() const override
            {
                return "AsyncClearMemoryLocation";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<ClearMemoryLocationResponse>(mResponse)->successCount =
                    mGrpcResponseBody.success_count();
                std::dynamic_pointer_cast<ClearMemoryLocationResponse>(mResponse)->failureCount =
                    mGrpcResponseBody.failure_count();

                std::unordered_set<int64_t> temporaryList;

                for (const auto& item : mGrpcResponseBody.failure_list())
                {
                    temporaryList.insert(item);
                }

                std::dynamic_pointer_cast<ClearMemoryLocationResponse>(mResponse)->failureList = {
                    temporaryList.begin(), temporaryList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<ClearMemoryLocationResponse>(mResponse)->failureList.clear();
            }

            void OnHasError() override
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<ClearMemoryLocationError>();
                    ConvertProtoToCommandError(error, *commandError);
                    commandError->failureList = { mGrpcResponseBody.failure_list().begin(),
                        mGrpcResponseBody.failure_list().end() };
                    mResponse->errors.push_back(commandError);
                }
            }

            MAKE_RESP_OVRD(ClearMemoryLocation);

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
            void FillGrpcRequest(const ClearMemoryLocationRequest& request)
            {
                *mGrpcRequestBody.mutable_location_list() = { request.locationList.begin(),
                    request.locationList.end() };
            }

            ptsl::ClearMemoryLocationRequestBody mGrpcRequestBody;
            ptsl::ClearMemoryLocationResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(ClearMemoryLocation, true, true);

        return std::dynamic_pointer_cast<ClearMemoryLocationResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
