// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionAudioRatePullSettings command.
 */

#include "Common.h"

const std::string g_pszSetSessionAudioRatePullSettings = "SetSessionAudioRatePullSettings";
const std::string g_pszSetSessionAudioRatePullSettingsHelp =
    "SetSessionAudioRatePullSettings -audio_rate_pull {SRP_None|SRP_Up01|SRP_Down01|SRP_Up4|SRP_Up4Up01|SRP_Up4Down01|SRP_Down4|SRP_Down4Up01|SRP_Down4Down01}";

PtslCmdCommandResult SetSessionAudioRatePullSettings(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionAudioRatePullSettingsRequest request = {};
    request.commandType = CommandType::SetSessionAudioRatePullSettings;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string audioRatePullParam = "audio_rate_pull";
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

            if (param == audioRatePullParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << audioRatePullParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SessionRatePull> audioRatePullMap = {
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_None),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Up01),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Down01),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Up4),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Up4Up01),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Up4Down01),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Down4),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Down4Up01),
                    RVRS_MAP_ENTRY(SessionRatePull, SRP_Down4Down01),
                };

                if (audioRatePullMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << audioRatePullParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.audioRatePull = audioRatePullMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionAudioRatePullSettings(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
