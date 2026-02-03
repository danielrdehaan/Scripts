// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::CloseSession command.
 */

#include "Common.h"

const std::string g_pszCloseSession = "CloseSession";
const std::string g_pszCloseSessionHelp = "CloseSession -save_on_close";

PtslCmdCommandResult CloseSession(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CloseSessionRequest request;
    request.commandType = CommandType::CloseSession;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.saveOnClose = false;

        if (params.size() > 0 && strcmp(params.at(0).c_str(), "-save_on_close") == 0)
        {
            request.saveOnClose = true;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.CloseSession(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
