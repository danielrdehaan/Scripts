// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RedoAll command.
 */

#include <date/date.h>

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<RedoAllResponse> CppPTSLClient::RedoAll(const CommandRequest& request)
    {
        struct RedoAllHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(RedoAll);

            std::string GetRequestName() const override
            {
                return "AsyncRedoAll";
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

                std::dynamic_pointer_cast<RedoAllResponse>(mResponse)->operations = { operationsTempList.begin(),
                    operationsTempList.end() };
            }

            MAKE_RESP_OVRD(RedoAll);

        protected:
            google::protobuf::Message& GetResponseBodyRef() override
            {
                return mGrpcResponseBody;
            }

        private:
            ptsl::RedoAllResponseBody mGrpcResponseBody;
        };

        MAKE_REQUEST(RedoAll, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return std::dynamic_pointer_cast<RedoAllResponse>(hndlr->GetResponse());
    }
} // namespace PTSLC_CPP
