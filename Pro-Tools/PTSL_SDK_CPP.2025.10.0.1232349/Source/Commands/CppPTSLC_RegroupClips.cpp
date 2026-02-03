// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RegroupClips command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RegroupClips(const CommandRequest& request)
    {
        struct RegroupClipsHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(RegroupClips);

            std::string GetRequestName() const override
            {
                return "AsyncRegroupClips";
            }
        };

        MAKE_REQUEST(RegroupClips, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
