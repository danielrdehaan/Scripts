// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionInterleavedState command.
 */

#include "Common.h"

const std::string g_pszGetSessionInterleavedState = "GetSessionInterleavedState";
const std::string g_pszGetSessionInterleavedStateHelp = g_pszGetSessionInterleavedState;

PtslCmdCommandResult GetSessionInterleavedState(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionInterleavedState;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionInterleavedStateResponse> rsp = client.GetSessionInterleavedState(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        const string bTrue = "interleaving";
        const string bFalse = "no_interleaving";

        cout << "GetSessionInterleavedState Response:" << endl;
        cout << "\t"
             << "current setting:"
             << "\t" << (rsp->currentSetting ? bTrue : bFalse) << endl;

        if (rsp->possibleSettings.size())
        {
            cout << "\t"
                 << "possible settings:" << endl;

            for (const bool& item : rsp->possibleSettings)
            {
                cout << "\t\t" << (item ? bTrue : bFalse) << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionInterleavedState Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionInterleavedState returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
