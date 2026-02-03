// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetTransportArmed command.
 */

#include "Common.h"

const std::string g_pszGetTransportArmed = "GetTransportArmed";
const std::string g_pszGetTransportArmedHelp = g_pszGetTransportArmed;

PtslCmdCommandResult GetTransportArmed(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetTransportArmed;

    // Call the client's method with the created request:
    std::shared_ptr<GetTransportArmedResponse> rsp = client.GetTransportArmed(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        const string bTrue = "transport_is_armed";
        const string bFalse = "no_transport_armed";

        cout << "GetTransportArmed Response:" << endl;
        cout << "\t"
             << "current setting:"
             << "\t" << (rsp->isTransportArmed ? bTrue : bFalse) << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetTransportArmed Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetTransportArmed returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
