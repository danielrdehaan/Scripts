// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::HostReadyCheck command.
 */

#include "Common.h"

const std::string g_pszHostReadyCheck = "HostReadyCheck";
const std::string g_pszHostReadyCheckHelp = g_pszHostReadyCheck;

PtslCmdCommandResult HostReadyCheck(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::HostReadyCheck;

    // Call the client's method with the created request:
    std::shared_ptr<HostReadyCheckResponse> rsp = client.HostReadyCheck(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "HostReadyCheck Response:" << endl;
        cout << "isHostReady:"
             << "\t" << rsp->isHostReady << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "HostReadyCheck Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "HostReadyCheck returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
