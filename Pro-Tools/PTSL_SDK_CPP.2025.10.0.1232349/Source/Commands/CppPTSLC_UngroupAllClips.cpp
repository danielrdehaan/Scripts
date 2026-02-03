// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::UngroupAllClips command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::UngroupAllClips(const CommandRequest& request)
    {
        struct UngroupAllClipsHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(UngroupAllClips);

            std::string GetRequestName() const override
            {
                return "AsyncUngroupAllClips";
            }
        };

        MAKE_REQUEST(UngroupAllClips, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
