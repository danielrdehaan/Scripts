// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::PlayHalfSpeed command.
 */

#include "Common.h"

const std::string g_pszPlayHalfSpeed = "PlayHalfSpeed";
const std::string g_pszPlayHalfSpeedHelp = g_pszPlayHalfSpeed;

PtslCmdCommandResult PlayHalfSpeed(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::PlayHalfSpeed;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.PlayHalfSpeed(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
