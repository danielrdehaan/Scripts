// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionLength command.
 */

#include "Common.h"

const std::string g_pszGetSessionLength = "GetSessionLength";
const std::string g_pszGetSessionLengthHelp = g_pszGetSessionLength;

PtslCmdCommandResult GetSessionLength(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionLength;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionLengthResponse> rsp = client.GetSessionLength(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionLength Response:" << endl;
        cout << "\t"
             << "session length:"
             << "\t" << rsp->sessionLength << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionLength Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionLength returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
