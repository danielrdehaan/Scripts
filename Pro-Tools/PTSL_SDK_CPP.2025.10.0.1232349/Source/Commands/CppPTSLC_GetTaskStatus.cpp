// Copyright 2021-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GetTaskStatus command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<GetTaskStatusResponse> CppPTSLClient::GetTaskStatus(const GetTaskStatusRequest& request)
    {
        struct GetTaskStatusHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(GetTaskStatus);

            std::string GetRequestName() const override
            {
                return "AsyncGetTaskStatus";
            }

            void OnHasBody() override
            {
                std::dynamic_pointer_cast<GetTaskStatusResponse>(mResponse)->requestedTaskId =
                    mGrpcResponseBody.task_id();
                std::dynamic_pointer_cast<GetTaskStatusResponse>(mResponse)->requestedTaskStatus.type =
                    static_cast<TaskStatus>(mGrpcResponseBody.status());
                std::dynamic_pointer_cast<GetTaskStatusResponse>(mResponse)->requestedTaskStatus.progress =
                    mGrpcResponseBody.progress();
            }

            MAKE_RESP_OVRD(GetTaskStatus);

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
            void FillGrpcRequest(const GetTaskStatusRequest& request)
            {
                mGrpcRequestBody.set_task_id(request.requestedTaskId);
            }

            ptsl::GetTaskStatusRequestBody mGrpcRequestBody;
            ptsl::GetTaskStatusResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(GetTaskStatus, true, false);

        return std::dynamic_pointer_cast<GetTaskStatusResponse>(hndlr->GetResponse());
    }
}; // namespace PTSLC_CPP
