// Copyright 2021-2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::SaveSession command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::SaveSession(const CommandRequest& request)
    {
        struct SaveSessionHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(SaveSession);

            std::string GetRequestName() const override
            {
                return "AsyncSaveSession";
            }

            bool IsNeedToPingTaskStatus() const override
            {
                return true;
            }
        };

        MAKE_REQUEST(SaveSession, false, true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
