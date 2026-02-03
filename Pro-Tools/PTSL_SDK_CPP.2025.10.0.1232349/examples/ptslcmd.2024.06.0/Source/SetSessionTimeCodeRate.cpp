// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionTimeCodeRate command.
 */

#include "Common.h"

const std::string g_pszSetSessionTimeCodeRate = "SetSessionTimeCodeRate";
const std::string g_pszSetSessionTimeCodeRateHelp =
    "SetSessionTimeCodeRate -time_code_rate {STCR_Fps23976|STCR_Fps24|STCR_Fps25|STCR_Fps2997|STCR_Fps2997Drop|STCR_Fps30|STCR_Fps30Drop|STCR_Fps47952|STCR_Fps48|STCR_Fps50|STCR_Fps5994|STCR_Fps5994Drop|STCR_Fps60|STCR_Fps60Drop|STCR_Fps100|STCR_Fps11988|STCR_Fps11988Drop|STCR_Fps120|STCR_Fps120Drop}";

PtslCmdCommandResult SetSessionTimeCodeRate(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionTimeCodeRateRequest request = {};
    request.commandType = CommandType::SetSessionTimeCodeRate;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string timeCodeRateParam = "time_code_rate";
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

            if (param == timeCodeRateParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << timeCodeRateParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SessionTimeCodeRate> timeCodeRateMap = {
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps23976),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps24),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps25),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps2997),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps2997Drop),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps30),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps30Drop),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps47952),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps48),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps50),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps5994),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps5994Drop),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps60),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps60Drop),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps100),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps11988),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps11988Drop),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps120),
                    RVRS_MAP_ENTRY(SessionTimeCodeRate, STCR_Fps120Drop),
                };

                if (timeCodeRateMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << timeCodeRateParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.timeCodeRate = timeCodeRateMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionTimeCodeRate(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
