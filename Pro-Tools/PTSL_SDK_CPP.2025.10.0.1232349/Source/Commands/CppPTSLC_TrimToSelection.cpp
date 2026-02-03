// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::TrimToSelection command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::TrimToSelection(const CommandRequest& request)
    {
        struct TrimToSelectionHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(TrimToSelection);

            std::string GetRequestName() const override
            {
                return "AsyncTrimToSelection";
            }
        };

        MAKE_REQUEST(TrimToSelection, false, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
