// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetMemoryLocationsManageMode command.
 */

#include "Common.h"

const string g_pszGetMemoryLocationsManageMode = "GetMemoryLocationsManageMode";
const string g_pszGetMemoryLocationsManageModeHelp = g_pszGetMemoryLocationsManageMode;

PtslCmdCommandResult GetMemoryLocationsManageMode(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetMemoryLocationsManageMode;

    // Call the client's method with the created request:
    std::shared_ptr<GetMemoryLocationsManageModeResponse> rsp = client.GetMemoryLocationsManageMode(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetMemoryLocationsManageMode Response:" << endl;

        cout << "\t"
             << "get link memory selection recall state:"
             << "\t" << rsp->enabled << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetMemoryLocationsManageMode Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetMemoryLocationsManageMode returned an unexpected status" << endl;
    }
    return PtslCmdCommandResult(rsp);
}
