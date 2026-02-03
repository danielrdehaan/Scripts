// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RedoAll command.
 */

#include <date/date.h>

#include "Common.h"

using namespace date;

const string g_pszRedoAll = "RedoAll";
const string g_pszRedoAllHelp = g_pszRedoAll;

PtslCmdCommandResult RedoAll(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::RedoAll;

    // Call the client's method with the created request:
    std::shared_ptr<RedoAllResponse> rsp = client.RedoAll(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "RedoAll Response:" << endl;

        if (rsp->operations.size())
        {
            cout << "\t"
                 << "operations:" << endl;
            for (auto& item : rsp->operations)
            {
                cout << "\t\ttime: " << item.time << endl;
                cout << "\t\toperation: " << item.operation << endl;
                cout << "\t\tdetails: " << item.details << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "RedoAll Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "RedoAll returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
