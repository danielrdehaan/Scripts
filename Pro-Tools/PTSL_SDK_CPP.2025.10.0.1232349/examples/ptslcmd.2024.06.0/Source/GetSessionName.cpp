// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionName command.
 */

#include "Common.h"

const std::string g_pszGetSessionName = "GetSessionName";
const std::string g_pszGetSessionNameHelp = g_pszGetSessionName;

PtslCmdCommandResult GetSessionName(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionName;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionNameResponse> rsp = client.GetSessionName(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionName Response:" << endl;
        cout << "\t"
             << "session name:"
             << "\t" << rsp->sessionName << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionName Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionName returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
