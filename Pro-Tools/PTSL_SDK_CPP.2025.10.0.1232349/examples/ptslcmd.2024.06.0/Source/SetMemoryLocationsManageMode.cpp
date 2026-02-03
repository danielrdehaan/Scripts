// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetMemoryLocationsManageMode command.
 */

#include "Common.h"

const string g_pszSetMemoryLocationsManageMode = "SetMemoryLocationsManageMode";
const string g_pszSetMemoryLocationsManageModeHelp = "SetMemoryLocationsManageMode -enabled <true|false>";

PtslCmdCommandResult SetMemoryLocationsManageMode(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetMemoryLocationsManageModeRequest request;
    request.commandType = CommandType::SetMemoryLocationsManageMode;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    {
        const string enabledParam = "enabled";
        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        bool hasEnabledParam = false;

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

            if (param == enabledParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << enabledParam << endl;
                    return false;
                }

                request.enabled = CommandLineParser::StringToBool(args.at(0));
                hasEnabledParam = true;
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }

        if (!hasEnabledParam)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " enabled" << endl;
            return PtslCmdCommandResult(false);
        }
    }

    // Call the client's method with the created request:
    return PtslCmdCommandResult(client.SetMemoryLocationsManageMode(request));
}
