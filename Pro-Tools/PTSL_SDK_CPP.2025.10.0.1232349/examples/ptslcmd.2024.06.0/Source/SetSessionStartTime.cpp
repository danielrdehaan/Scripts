// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionStartTime command.
 */

#include "Common.h"

const std::string g_pszSetSessionStartTime = "SetSessionStartTime";
const std::string g_pszSetSessionStartTimeHelp =
    "SetSessionStartTime -session_start_time <session_start_time> -track_offset_opts {BarsBeats|MinSecs|FeetFrames|Samples|TimeCode} [-maintain_relative_position]";

PtslCmdCommandResult SetSessionStartTime(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionStartTimeRequest request = {};
    request.commandType = CommandType::SetSessionStartTime;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string sessionStartTimeParam = "session_start_time";
        const string trackOffsetOptsParam = "track_offset_opts";
        const string maintainRelativePositionParam = "maintain_relative_position";

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

            if (param == sessionStartTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << sessionStartTimeParam << endl;
                    return false;
                }

                request.sessionStartTime = args.at(0);
            }
            else if (param == trackOffsetOptsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackOffsetOptsParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, TrackOffsetOptions> trackOffsetOptsParamMap = {
                    RVRS_MAP_ENTRY(TrackOffsetOptions, BarsBeats),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, MinSecs),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, FeetFrames),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, Samples),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, TimeCode),
                };

                if (trackOffsetOptsParamMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackOffsetOptsParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.trackOffsetOpts = trackOffsetOptsParamMap.at(theArg);
            }
            else if (param == maintainRelativePositionParam)
            {
                if (!args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << maintainRelativePositionParam
                         << endl;
                    return false;
                }

                request.maintainRelativePosition = true;
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionStartTime(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
