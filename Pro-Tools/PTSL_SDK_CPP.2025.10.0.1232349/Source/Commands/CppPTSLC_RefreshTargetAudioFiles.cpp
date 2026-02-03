// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RefreshTargetAudioFiles command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<RefreshTargetAudioFilesResponse> CppPTSLClient::RefreshTargetAudioFiles(
        const RefreshTargetAudioFilesRequest& request)
    {
        struct RefreshTargetAudioFilesHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(RefreshTargetAudioFiles);

            std::string GetRequestName() const override
            {
                return "AsyncRefreshTargetAudioFiles";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<RefreshTargetAudioFilesResponse>(mResponse)->successCount =
                    mGrpcResponseBody.success_count();
                std::dynamic_pointer_cast<RefreshTargetAudioFilesResponse>(mResponse)->failureCount =
                    mGrpcResponseBody.failure_count();

                std::vector<std::string> temporaryList;

                for (const auto& item : mGrpcResponseBody.failure_list())
                {
                    temporaryList.push_back(item);
                }

                std::dynamic_pointer_cast<RefreshTargetAudioFilesResponse>(mResponse)->failureList = {
                    temporaryList.begin(), temporaryList.end()
                };
            }

            void OnNoBody() override
            {
                std::dynamic_pointer_cast<RefreshTargetAudioFilesResponse>(mResponse)->failureList.resize(0);
            }

            void OnHasError() override
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<RefreshTargetAudioFilesError>();
                    ConvertProtoToCommandError(error, *commandError);
                    commandError->failureList = { mGrpcResponseBody.failure_list().begin(),
                        mGrpcResponseBody.failure_list().end() };
                    mResponse->errors.push_back(commandError);
                }
            }

            MAKE_RESP_OVRD(RefreshTargetAudioFiles);

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
            void FillGrpcRequest(const RefreshTargetAudioFilesRequest& request)
            {
                *mGrpcRequestBody.mutable_file_list() = { request.filesList.begin(), request.filesList.end() };
            }

            ptsl::RefreshTargetAudioFilesRequestBody mGrpcRequestBody;
            ptsl::RefreshTargetAudioFilesResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(RefreshTargetAudioFiles, true, true);

        return std::dynamic_pointer_cast<RefreshTargetAudioFilesResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
