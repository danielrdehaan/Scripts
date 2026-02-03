// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionFeetFramesRate command.
 */

#include "Common.h"

const std::string g_pszSetSessionFeetFramesRate = "SetSessionFeetFramesRate";
const std::string g_pszSetSessionFeetFramesRateHelp =
    "SetSessionFeetFramesRate -feet_frames_rate {SFFR_Fps23976|SFFR_Fps24|SFFR_Fps25}";

PtslCmdCommandResult SetSessionFeetFramesRate(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionFeetFramesRateRequest request = {};
    request.commandType = CommandType::SetSessionFeetFramesRate;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string feetFramesRateParam = "feet_frames_rate";
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

            if (param == feetFramesRateParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << feetFramesRateParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SessionFeetFramesRate> feetFramesRateMap = {
                    RVRS_MAP_ENTRY(SessionFeetFramesRate, SFFR_Fps23976),
                    RVRS_MAP_ENTRY(SessionFeetFramesRate, SFFR_Fps24),
                    RVRS_MAP_ENTRY(SessionFeetFramesRate, SFFR_Fps25),
                };

                if (feetFramesRateMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << feetFramesRateParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.feetFramesRate = feetFramesRateMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionFeetFramesRate(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
