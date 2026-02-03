// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::ConsolidateClip command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::ConsolidateClip(const CommandRequest& request)
    {
        struct ConsolidateClipHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(ConsolidateClip);

            std::string GetRequestName() const override
            {
                return "AsyncConsolidateClips";
            }
        };

        MAKE_REQUEST(ConsolidateClip, false, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
