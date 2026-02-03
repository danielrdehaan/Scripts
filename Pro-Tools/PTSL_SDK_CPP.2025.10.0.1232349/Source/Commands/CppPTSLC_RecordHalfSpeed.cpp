// Copyright 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the PTSLC_CPP::CppPTSLClient::RecordHalfSpeed command.
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    std::shared_ptr<CommandResponse> CppPTSLClient::RecordHalfSpeed(const CommandRequest& request)
    {
        struct RecordHalfSpeedHandler : public DefaultRequestHandler
        {
            INIT_HNDLR(RecordHalfSpeed);

            std::string GetRequestName() const override
            {
                return "AsyncRecordHalfSpeed";
            }
        };

        MAKE_REQUEST(RecordHalfSpeed, /*HAS_REQ*/ false, /*IS_STREAMING*/ true);

        return hndlr->GetResponse();
    }
}; // namespace PTSLC_CPP
