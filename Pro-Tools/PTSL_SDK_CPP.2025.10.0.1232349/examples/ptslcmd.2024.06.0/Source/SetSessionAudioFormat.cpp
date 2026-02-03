// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionAudioFormat command.
 */

#include "Common.h"

const std::string g_pszSetSessionAudioFormat = "SetSessionAudioFormat";
const std::string g_pszSetSessionAudioFormatHelp = "SetSessionAudioFormat -audio_format {SAF_WAVE|SAF_AIFF}";

PtslCmdCommandResult SetSessionAudioFormat(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionAudioFormatRequest request = {};
    request.commandType = CommandType::SetSessionAudioFormat;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string audioFormatParam = "audio_format";
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

            if (param == audioFormatParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << audioFormatParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SessionAudioFormat> audioFormatMap = {
                    RVRS_MAP_ENTRY(SessionAudioFormat, SAF_AIFF),
                    RVRS_MAP_ENTRY(SessionAudioFormat, SAF_WAVE),
                };

                if (audioFormatMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << audioFormatParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.audioFormat = audioFormatMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionAudioFormat(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
