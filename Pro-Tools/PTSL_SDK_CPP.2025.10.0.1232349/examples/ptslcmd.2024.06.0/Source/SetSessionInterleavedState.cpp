// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionInterleavedState command.
 */

#include "Common.h"

const std::string g_pszSetSessionInterleavedState = "SetSessionInterleavedState";
const std::string g_pszSetSessionInterleavedStateHelp = "SetSessionInterleavedState -[no_]interleaving";

PtslCmdCommandResult SetSessionInterleavedState(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionInterleavedStateRequest request = {};
    request.commandType = CommandType::SetSessionInterleavedState;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string interleavedStateParam = "interleaving";
        const string noModifier = "no_";

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

            if (param.size() >= interleavedStateParam.size()
                && 0
                    == param.compare(param.size() - interleavedStateParam.size(),
                        interleavedStateParam.size(),
                        interleavedStateParam)) // if ends with "interleaving"
            {
                if (!args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << interleavedStateParam << endl;
                    return false;
                }

                request.interleavedState = param.at(0) != noModifier.at(0);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionInterleavedState(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
