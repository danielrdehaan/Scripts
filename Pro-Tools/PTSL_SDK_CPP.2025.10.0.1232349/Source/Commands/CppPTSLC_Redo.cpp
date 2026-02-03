// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::Redo command.
 */

#include <date/date.h>

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<RedoResponse> CppPTSLClient::Redo(const RedoRequest& request)
    {
        struct RedoHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(Redo);

            std::string GetRequestName() const override
            {
                return "AsyncRedo";
            }

            void OnHasBody() override
            {
                std::vector<UndoHistoryOperation> operationsTempList;
                for (const auto& item : mGrpcResponseBody.operations())
                {
                    UndoHistoryOperation operation;
                    std::istringstream { item.time() } >> date::parse("%FT%T%z", operation.time);
                    operation.operation = item.operation();
                    operation.details = item.details();
                    operationsTempList.push_back(operation);
                }

                std::dynamic_pointer_cast<RedoResponse>(mResponse)->operations = { operationsTempList.begin(),
                    operationsTempList.end() };
            }

            MAKE_RESP_OVRD(Redo);

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
            void FillGrpcRequest(const RedoRequest& request)
            {
                mGrpcRequestBody.set_levels(request.levels);
            }

            ptsl::RedoRequestBody mGrpcRequestBody;
            ptsl::RedoResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(Redo, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<RedoResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
