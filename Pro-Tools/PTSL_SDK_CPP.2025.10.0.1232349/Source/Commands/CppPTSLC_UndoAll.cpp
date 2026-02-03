// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::UndoAll command.
 */

#include <date/date.h>

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<UndoAllResponse> CppPTSLClient::UndoAll(const CommandRequest& request)
    {
        struct UndoAllHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(UndoAll);

            std::string GetRequestName() const override
            {
                return "AsyncUndoAll";
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

                std::dynamic_pointer_cast<UndoAllResponse>(mResponse)->operations = { operationsTempList.begin(),
                    operationsTempList.end() };
            }

            MAKE_RESP_OVRD(UndoAll);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::UndoAllResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(UndoAll, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<UndoAllResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
