// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SaveSession command.
 */

#include "Common.h"

const std::string g_pszSaveSession = "SaveSession";
const std::string g_pszSaveSessionHelp = g_pszSaveSession;

PtslCmdCommandResult SaveSession(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::SaveSession;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SaveSession(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
