// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ClearUndoQueue command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ClearUndoQueue(const CommandRequest& request)
    {
        struct ClearUndoQueueHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(ClearUndoQueue);

            std::string GetRequestName() const override
            {
                return "AsyncClearUndoQueue";
            }
        };

        MAKE_REQUEST(ClearUndoQueue, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
