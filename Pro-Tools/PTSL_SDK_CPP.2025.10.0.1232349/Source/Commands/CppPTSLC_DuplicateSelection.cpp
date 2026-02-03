// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::DuplicateSelection command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::DuplicateSelection(const CommandRequest& request)
    {
        struct DuplicateSelectionHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(DuplicateSelection);

            std::string GetRequestName() const override
            {
                return "AsyncDuplicateSelection";
            }
        };

        MAKE_REQUEST(DuplicateSelection, false, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
