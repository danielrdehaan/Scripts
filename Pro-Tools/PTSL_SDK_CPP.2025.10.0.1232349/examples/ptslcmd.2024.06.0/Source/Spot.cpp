// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::Spot command.
 */

#include "Common.h"

const std::string g_pszSpot = "Spot";
const std::string g_pszSpotHelp =
    "Spot [-location_options {BarsBeats|MinSecs|FeetFrames|Samples|TimeCode}] [-location_type {Start|SyncPoint|End}] -location_value <location_value>";

PtslCmdCommandResult Spot(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SpotRequest request;
    request.commandType = CommandType::Spot;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.spotLocationData.locationOptions = TrackOffsetOptions::MinSecs;
        request.spotLocationData.locationType = SpotLocationType::Start;
        request.spotLocationData.locationValue = "00:00:00:00";

        const string offsetOptionsParam = "location_options";
        const string locationTypeParam = "location_type";
        const string locationValueParam = "location_value";

        // Separate each command line parameter's arguments:
        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        // Make a request accordingly to command line params:
        for (const auto& pair : paramsArgsMap)
        {
            const string param = pair.first;
            const vector<string> args = pair.second;

            if (param == CommandLineParser::PARAMETERLESS_ARGS_KEY && !args.empty())
            {
                cout << CommandLineParser::PARAMETERLESS_ARGS << " " << args.at(0) << endl;
                return false;
            }

            if (param == offsetOptionsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << offsetOptionsParam << endl;
                    return false;
                }

                map<string, TrackOffsetOptions> trackOffsetOptsParamMap = {
                    RVRS_MAP_ENTRY(TrackOffsetOptions, BarsBeats),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, MinSecs),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, FeetFrames),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, Samples),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, TimeCode),
                };

                const string theArg = args.at(0);
                if (trackOffsetOptsParamMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << offsetOptionsParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.spotLocationData.locationOptions = trackOffsetOptsParamMap.at(theArg);
            }
            else if (param == locationTypeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationTypeParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SpotLocationType> spotLocTypeParamMap = {
                    RVRS_MAP_ENTRY(SpotLocationType, Start),
                    RVRS_MAP_ENTRY(SpotLocationType, SyncPoint),
                    RVRS_MAP_ENTRY(SpotLocationType, End),
                };

                if (spotLocTypeParamMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationTypeParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.spotLocationData.locationType = spotLocTypeParamMap.at(theArg);
            }
            else if (param == locationValueParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_MISSED << " " << locationValueParam << endl;
                    return false;
                }
                request.spotLocationData.locationValue = args.at(0);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        } // (end for)
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.Spot(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
