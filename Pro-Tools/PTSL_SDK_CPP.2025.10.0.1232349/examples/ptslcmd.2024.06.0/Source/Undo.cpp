// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::Undo command.
 */

#include <date/date.h>

#include "Common.h"

using namespace date;

const string g_pszUndo = "Undo";
const string g_pszUndoHelp = "Undo -levels <levels>";

PtslCmdCommandResult Undo(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    UndoRequest request;
    request.commandType = CommandType::Undo;
    request.levels = 0;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    {
        const string levelsParam = "levels";
        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        // Populate the request by the parameters and their args provided:
        for (const auto& pair : paramsArgsMap)
        {
            const string param = pair.first;
            const vector<string> args = pair.second;

            if (param == CommandLineParser::PARAMETERLESS_ARGS_KEY && !args.empty())
            {
                cout << CommandLineParser::PARAMETERLESS_ARGS << " " << args.at(0) << endl;
                return false;
            }

            if (param == levelsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << levelsParam << endl;
                    return false;
                }

                request.levels = std::stoi(args.at(0));
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<UndoResponse> rsp = client.Undo(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "Undo Response:" << endl;

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
        cout << "Undo Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "Undo returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
