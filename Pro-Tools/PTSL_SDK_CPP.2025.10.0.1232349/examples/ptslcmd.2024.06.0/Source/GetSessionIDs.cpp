// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionIDs command.
 */

#include "Common.h"

const string g_pszGetSessionIDs = "GetSessionIDs";
const string g_pszGetSessionIDsHelp = g_pszGetSessionIDs;

PtslCmdCommandResult GetSessionIDs(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetSessionIDs;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionIDsResponse> rsp = client.GetSessionIDs(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionIDs Response:" << endl;

        cout << "\t"
             << "Session origin ID:"
             << "\t" << static_cast<std::string>(rsp->originId) << endl;

        cout << "\t"
             << "Session instance ID:"
             << "\t" << static_cast<std::string>(rsp->instanceId) << endl;

        cout << "\t"
             << "Session parent ID:"
             << "\t" << static_cast<std::string>(rsp->parentId) << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionIDs Request Failed." << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionIDs returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
