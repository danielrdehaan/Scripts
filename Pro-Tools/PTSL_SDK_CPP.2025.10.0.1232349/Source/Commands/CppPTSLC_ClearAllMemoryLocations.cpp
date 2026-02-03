// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ClearAllMemoryLocations command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<ClearAllMemoryLocationsResponse> CppPTSLClient::ClearAllMemoryLocations(
        const CommandRequest& request)
    {
        struct ClearAllMemoryLocationsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(ClearAllMemoryLocations);

            std::string GetRequestName() const override
            {
                return "AsyncClearAllMemoryLocations";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<ClearAllMemoryLocationsResponse>(mResponse)->successCount =
                    mGrpcResponseBody.success_count();
                std::dynamic_pointer_cast<ClearAllMemoryLocationsResponse>(mResponse)->failureCount =
                    mGrpcResponseBody.failure_count();
                std::unordered_set<int64_t> failureListTempList;
                for (const auto& item : mGrpcResponseBody.failure_list())
                {
                    failureListTempList.insert(item);
                }

                std::dynamic_pointer_cast<ClearAllMemoryLocationsResponse>(mResponse)->failureList = {
                    failureListTempList.begin(), failureListTempList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<ClearAllMemoryLocationsResponse>(mResponse)->failureList.clear();
            }

            void OnHasError() override
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<ClearAllMemoryLocationsError>();
                    ConvertProtoToCommandError(error, *commandError);
                    commandError->failureList = { mGrpcResponseBody.failure_list().begin(),
                        mGrpcResponseBody.failure_list().end() };
                    mResponse->errors.push_back(commandError);
                }
            }

            MAKE_RESP_OVRD(ClearAllMemoryLocations);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::ClearAllMemoryLocationsResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(ClearAllMemoryLocations, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);

        return std::dynamic_pointer_cast<ClearAllMemoryLocationsResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
