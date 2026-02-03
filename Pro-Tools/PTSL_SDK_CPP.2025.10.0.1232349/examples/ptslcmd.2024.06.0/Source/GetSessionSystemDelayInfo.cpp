// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionSystemDelayInfo command.
 */

#include "Common.h"

const string g_pszGetSessionSystemDelayInfo = "GetSessionSystemDelayInfo";
const string g_pszGetSessionSystemDelayInfoHelp = g_pszGetSessionSystemDelayInfo;

PtslCmdCommandResult GetSessionSystemDelayInfo(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetSessionSystemDelayInfo;

    // Call the client's method with the created request:
    shared_ptr<GetSessionSystemDelayInfoResponse> rsp = client.GetSessionSystemDelayInfo(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionSystemDelayInfo Response:" << endl;

        cout << "\t"
             << "Samples:"
             << "\t" << rsp->samples << endl;

        cout << "\t"
             << "Delay compensation:"
             << "\t" << (rsp->delayCompensationEnabled ? "enabled" : "disabled") << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionSystemDelayInfo Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionSystemDelayInfo returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
