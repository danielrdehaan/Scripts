// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetTrackOnlineState command.
 */

#include "Common.h"

const string g_pszSetTrackOnlineState = "SetTrackOnlineState";
const string g_pszSetTrackOnlineStateHelp = "SetTrackOnlineState -track_name <track_name> -enabled <true|false>";

PtslCmdCommandResult SetTrackOnlineState(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetTrackOnlineStateRequest request;
    request.commandType = CommandType::SetTrackOnlineState;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string trackNameParam = "track_name";
        const string enabledParam = "enabled";
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

            if (param == trackNameParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackNameParam << endl;
                    return false;
                }

                request.trackName = args.at(0);
            }
            else if (param == enabledParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << enabledParam << endl;
                    return false;
                }

                request.enabled = CommandLineParser::StringToBool(args.at(0));
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetTrackOnlineState(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
