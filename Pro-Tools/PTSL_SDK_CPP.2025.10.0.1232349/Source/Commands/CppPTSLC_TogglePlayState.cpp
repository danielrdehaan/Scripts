// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::TogglePlayState command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::TogglePlayState(const CommandRequest& request)
    {
        struct TogglePlayStateHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(TogglePlayState);

            std::string GetRequestName() const override
            {
                return "AsyncTogglePlayState";
            }
        };

        MAKE_REQUEST(TogglePlayState, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
