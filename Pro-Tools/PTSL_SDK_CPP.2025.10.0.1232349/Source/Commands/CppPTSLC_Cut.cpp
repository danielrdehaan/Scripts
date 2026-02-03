// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::Cut command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::Cut(const CommandRequest& request)
    {
        struct CutHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(Cut);

            std::string GetRequestName() const override
            {
                return "AsyncCut";
            }
        };

        MAKE_REQUEST(Cut, false, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
