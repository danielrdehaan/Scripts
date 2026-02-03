// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::Undo command.
 */

#include <date/date.h>

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<UndoResponse> CppPTSLClient::Undo(const UndoRequest& request)
    {
        struct UndoHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR_OVRD(Undo);

            std::string GetRequestName() const override
            {
                return "AsyncUndo";
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

                std::dynamic_pointer_cast<UndoResponse>(mResponse)->operations = { operationsTempList.begin(),
                    operationsTempList.end() };
            }

            MAKE_RESP_OVRD(Undo);

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
            void FillGrpcRequest(const UndoRequest& request)
            {
                mGrpcRequestBody.set_levels(request.levels);
            }

            ptsl::UndoRequestBody mGrpcRequestBody;
            ptsl::UndoResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(Undo, /*HAS_REQ*/ true, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<UndoResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
