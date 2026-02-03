// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetTimelineSelection command.
 */

#include "Common.h"

const std::string g_pszSetTimelineSelection = "SetTimelineSelection";
const std::string g_pszSetTimelineSelectionHelp =
    "SetTimelineSelection [-play_start_marker_time <play_start_marker_time>] -in_time <in_time> [-out_time <out_time>] [-pre_roll_start_time <pre_roll_start_time>] [-post_roll_stop_time <post_roll_stop_time>] [-pre_roll_enabled {TB_None|TB_True|TB_False}] [-post_roll_enabled {TB_None|TB_True|TB_False}] [-update_video_to {TUV_None|TUV_In|TUV_Out}] [-propagate_to_satellites {TB_None|TB_True|TB_False}]";

PtslCmdCommandResult SetTimelineSelection(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SetTimelineSelectionRequest request;
    request.commandType = CommandType::SetTimelineSelection;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string playStartMarkerTimeParam = "play_start_marker_time";
        const string inTimeParam = "in_time";
        const string outTimeParam = "out_time";
        const string preRollStartTimeParam = "pre_roll_start_time";
        const string postRollStopTimeParam = "post_roll_stop_time";
        const string preRollEnabledParam = "pre_roll_enabled";
        const string postRollEnabledParam = "post_roll_enabled";
        const string updateVideoToParam = "update_video_to";
        const string propagateToSatellitesParam = "propagate_to_satellites";

        bool missedInTime = true;

        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        for (const auto& pair : paramsArgsMap)
        {
            const string param = pair.first;
            const vector<string> args = pair.second;

            if (param == CommandLineParser::PARAMETERLESS_ARGS_KEY && !args.empty())
            {
                cout << CommandLineParser::PARAMETERLESS_ARGS << " " << args.at(0) << endl;
                return false;
            }

            if (param == playStartMarkerTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << playStartMarkerTimeParam << endl;
                    return false;
                }

                request.playStartMarkerTime = args.at(0);
            }
            else if (param == inTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << inTimeParam << endl;
                    return false;
                }

                request.inTime = args.at(0);
                missedInTime = false;
            }
            else if (param == outTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << outTimeParam << endl;
                    return false;
                }

                request.outTime = args.at(0);
            }
            else if (param == preRollStartTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << preRollStartTimeParam << endl;
                    return false;
                }

                request.preRollStartTime = args.at(0);
            }
            else if (param == postRollStopTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << postRollStopTimeParam << endl;
                    return false;
                }

                request.postRollStopTime = args.at(0);
            }
            else if (param == preRollEnabledParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << preRollEnabledParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TripleBool>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << preRollEnabledParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.preRollEnabled = theValue.value();
            }
            else if (param == postRollEnabledParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << postRollEnabledParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TripleBool>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << postRollEnabledParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.postRollEnabled = theValue.value();
            }
            else if (param == updateVideoToParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << updateVideoToParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TimelineUpdateVideo>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << updateVideoToParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.updateVideoTo = theValue.value();
            }
            else if (param == propagateToSatellitesParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << propagateToSatellitesParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TripleBool>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << propagateToSatellitesParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.propagateToSatellites = theValue.value();
            }
        }

        if (missedInTime)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (missedInTime)
            {
                cout << " in_time";
            }

            cout << endl;

            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetTimelineSelection(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
