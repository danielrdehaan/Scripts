// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RecallZoomPreset command.
 */

#include "Common.h"

const string g_pszRecallZoomPreset = "RecallZoomPreset";
const string g_pszRecallZoomPresetHelp = "RecallZoomPreset -zoom_preset {1|2|3|4|5}";

PtslCmdCommandResult RecallZoomPreset(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    RecallZoomPresetRequest request;
    request.commandType = CommandType::RecallZoomPreset;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string zoomPresetParam = "zoom_preset";
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

            if (param == zoomPresetParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << zoomPresetParam << endl;
                    return false;
                }

                request.zoomPreset = std::stoi(args.at(0));
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.RecallZoomPreset(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
