// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::UndoAll command.
 */

#include <date/date.h>

#include "Common.h"

using namespace date;

const string g_pszUndoAll = "UndoAll";
const string g_pszUndoAllHelp = g_pszUndoAll;

PtslCmdCommandResult UndoAll(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::UndoAll;

    // Call the client's method with the created request:
    std::shared_ptr<UndoAllResponse> rsp = client.UndoAll(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "UndoAll Response:" << endl;

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
        cout << "UndoAll Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "UndoAll returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
