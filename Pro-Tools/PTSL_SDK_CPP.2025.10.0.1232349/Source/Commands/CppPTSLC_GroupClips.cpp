// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::GroupClips command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::GroupClips(const CommandRequest& request)
    {
        struct GroupClipsHandler : public DefaultRequestHandler
        {
        public:
            INIT_HNDLR(GroupClips);

            std::string GetRequestName() const override
            {
                return "AsyncGroupClips";
            }
        };

        MAKE_REQUEST(GroupClips, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);
        return hndlr->GetResponse();
    }
} // namespace PTSLC_CPP
