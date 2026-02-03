// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionVideoRatePullSettings command.
 */

#include "Common.h"

const std::string g_pszSetSessionVideoRatePullSettings = "SetSessionVideoRatePullSettings";
const std::string g_pszSetSessionVideoRatePullSettingsHelp =
    "SetSessionVideoRatePullSettings -video_rate_pull {SRP_None|SRP_Up01|SRP_Down01}";

PtslCmdCommandResult SetSessionVideoRatePullSettings(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionVideoRatePullSettingsRequest request = {};
    request.commandType = CommandType::SetSessionVideoRatePullSettings;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string videoRatePullParam = "video_rate_pull";
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

            if (param == videoRatePullParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << videoRatePullParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SessionRatePull> videoRatePullMap = {
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_None),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Up01),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Down01),
                };

                if (videoRatePullMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << videoRatePullParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.videoRatePull = videoRatePullMap[theArg];
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionVideoRatePullSettings(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
