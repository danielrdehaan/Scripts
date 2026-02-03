// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetTimelineSelection command.
 */

#include "Common.h"

const string g_pszGetTimelineSelection = "GetTimelineSelection";
const string g_pszGetTimelineSelectionHelp =
    "GetTimelineSelection -time_scale {BarsBeats|MinSecs|TimeCode|FeetFrames|Samples}";

PtslCmdCommandResult GetTimelineSelection(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    GetTimelineSelectionRequest request;
    request.commandType = CommandType::GetTimelineSelection;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool missedTimeScale = true;

        const string timeScaleParam = "time_scale";
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

            if (param == timeScaleParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << timeScaleParam << endl;
                    return false;
                }

                const string theArg = args.at(0);
                const auto theValue = StringToEnum<TrackOffsetOptions>(theArg);
                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << timeScaleParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.timeScale = theValue.value();
                missedTimeScale = false;
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }

        if (missedTimeScale)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " time_scale" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<GetTimelineSelectionResponse> rsp = client.GetTimelineSelection(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetTimelineSelection Response:" << endl;
        cout << "\t"
             << "play state marker time:"
             << "\t" << rsp->playStartMarkerTime << endl;
        cout << "\t"
             << "in time:"
             << "\t" << rsp->inTime << endl;
        cout << "\t"
             << "out time:"
             << "\t" << rsp->outTime << endl;
        cout << "\t"
             << "preroll start time:"
             << "\t" << rsp->preRollStartTime << endl;
        cout << "\t"
             << "post roll stop time:"
             << "\t" << rsp->postRollStopTime << endl;
        cout << "\t"
             << "preroll enabled:"
             << "\t" << rsp->preRollEnabled << endl;
        cout << "\t"
             << "postroll enabled:"
             << "\t" << rsp->postRollEnabled << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetTimelineSelection Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetTimelineSelection returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
