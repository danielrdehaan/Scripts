// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetMainCounterFormat command.
 */

#include "Common.h"

const string g_pszSetMainCounterFormat = "SetMainCounterFormat";
const string g_pszSetMainCounterFormatHelp =
    "SetMainCounterFormat [-time_scale {BarsBeats|MinSecs|TimeCode|FeetFrames|Samples}]";

PtslCmdCommandResult SetMainCounterFormat(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetMainCounterFormatRequest request;
    request.commandType = CommandType::SetMainCounterFormat;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    {
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
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    auto rsp = client.SetMainCounterFormat(request);

    return PtslCmdCommandResult(rsp);
}
