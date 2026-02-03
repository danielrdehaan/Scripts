// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::TogglePlayState command.
 */

#include "Common.h"

const std::string g_pszTogglePlayState = "TogglePlayState";
const std::string g_pszTogglePlayStateHelp = g_pszTogglePlayState;

PtslCmdCommandResult TogglePlayState(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::TogglePlayState;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.TogglePlayState(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
