// Copyright 2020, 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::OpenSession command.
 */

#include "Common.h"

const std::string g_pszOpenSession = "OpenSession";
const std::string g_pszOpenSessionHelp = "OpenSession -path <path>";

PtslCmdCommandResult OpenSession(const std::vector<std::string>& params, CppPTSLClient& client)
{
    OpenSessionRequest request;
    request.commandType = CommandType::OpenSession;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        if (params.size() > 1 && strcmp(params.at(0).c_str(), "-path") == 0)
        {
            request.sessionPath = params.at(1);
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " path" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.OpenSession(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
