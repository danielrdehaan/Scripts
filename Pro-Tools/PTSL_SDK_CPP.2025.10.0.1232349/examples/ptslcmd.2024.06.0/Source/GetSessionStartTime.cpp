// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionStartTime command.
 */

#include "Common.h"

const std::string g_pszGetSessionStartTime = "GetSessionStartTime";
const std::string g_pszGetSessionStartTimeHelp = g_pszGetSessionStartTime;

PtslCmdCommandResult GetSessionStartTime(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionStartTime;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionStartTimeResponse> rsp = client.GetSessionStartTime(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionStartTime Response:" << endl;
        cout << "\t"
             << "session start time:"
             << "\t" << rsp->sessionStartTime << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionStartTime Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionStartTime returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
