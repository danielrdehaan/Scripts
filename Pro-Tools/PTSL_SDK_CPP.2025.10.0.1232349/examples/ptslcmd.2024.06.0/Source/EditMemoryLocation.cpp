// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::EditMemoryLocation command.
 */

#include "Common.h"
const string g_pszEditMemoryLocation = "EditMemoryLocation";
const string g_pszEditMemoryLocationHelp =
    "EditMemoryLocation -number <number> -name <name> -start_time <start_time> [-end_time <end_time>] -time_properties {TP_Marker|TP_Selection|TP_None} -reference {MLR_BarBeat|MLR_Absolute|MLR_FollowTrackTimebase} -general_properties <0/1 flags: zoom_settings pre_post_roll_times track_visibility track_heights group_enables window_configuration e.g. 1 0 0 1 1 0> [ -window_configuration_index <window_configuration_index> ] [-comments <comments>] [-color_index <color>] [-location {MLC_MainRuler|MLC_Track}] [-track_name <track_name>] [-venue_snapshot_index <venue_snapshot_index>] [-venue_snapshot_name <venue_snapshot_name>]";

PtslCmdCommandResult EditMemoryLocation(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    EditMemoryLocationRequest request;
    request.commandType = CommandType::EditMemoryLocation;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.comments = "";
        request.colorIndex = 0;
        request.generalProperties.venueSnapshotIndex = 0;

        const string numberParam = "number";
        const string nameParam = "name";
        const string startTimeParam = "start_time";
        const string endTimeParam = "end_time";
        const string timePropertiesParam = "time_properties";
        const string referenceParam = "reference";
        const string generalPropertiesParam = "general_properties";
        const int generalPropertiesCnt = 6;
        const string windowConfigurationIndexParam = "window_configuration_index";
        const string commentsParam = "comments";
        const string colorIndexParam = "color_index";
        const string locationParam = "location";
        const string trackNameParam = "track_name";
        const string venueSnapshotIndex = "venue_snapshot_index";
        const string venueSnapshotName = "venue_snapshot_name";

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

            if (param == numberParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << numberParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                if (!CommandLineParser::IsNumber(theArg))
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << numberParam
                         << ". Wrong number: " << theArg << endl;
                    return false;
                }

                request.number = std::stoi(theArg);
            }
            else if (param == nameParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << nameParam << endl;
                    return false;
                }

                const string theArg = args.at(0);
                request.name = theArg;
            }
            else if (param == startTimeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << startTimeParam << endl;
                    return false;
                }

                request.startTime = args.at(0);
            }
            else if (param == endTimeParam)
            {
                request.endTime = args.at(0);
            }
            else if (param == timePropertiesParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << timePropertiesParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, TimeProperties> timePropertiesMap = {
                    RVRS_MAP_ENTRY(TimeProperties, TP_Marker),
                    RVRS_MAP_ENTRY(TimeProperties, TP_Selection),
                    RVRS_MAP_ENTRY(TimeProperties, TP_None),
                };

                if (timePropertiesMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << timePropertiesParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.timeProperties = timePropertiesMap[theArg];
            }
            else if (param == referenceParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << referenceParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, MemoryLocationReference> referenceMap = {
                    RVRS_MAP_ENTRY(MemoryLocationReference, MLR_BarBeat),
                    RVRS_MAP_ENTRY(MemoryLocationReference, MLR_Absolute),
                    RVRS_MAP_ENTRY(MemoryLocationReference, MLR_FollowTrackTimebase),
                };

                if (referenceMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << referenceParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.reference = referenceMap[theArg];
            }
            else if (param == generalPropertiesParam)
            {
                if (args.size() != generalPropertiesCnt)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << generalPropertiesParam
                         << " General properties flags number should be 6." << endl;
                    return false;
                }

                for (auto flag : args)
                {
                    if (!CommandLineParser::IsNumber(flag))
                    {
                        cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << generalPropertiesParam
                             << " General properties flags should be numbers: " << flag << endl;
                        return false;
                    }
                }

                request.generalProperties.zoomSettings = std::stoi(args.at(0));
                request.generalProperties.prePostRollTimes = std::stoi(args.at(1));
                request.generalProperties.trackVisibility = std::stoi(args.at(2));
                request.generalProperties.trackHeights = std::stoi(args.at(3));
                request.generalProperties.groupEnables = std::stoi(args.at(4));
                request.generalProperties.windowConfiguration = std::stoi(args.at(5));
            }
            else if (param == windowConfigurationIndexParam)
            {
                const string theArg = args.at(0);
                request.generalProperties.windowConfigurationIndex = std::stoi(theArg);
            }
            else if (param == commentsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << commentsParam << endl;
                    return false;
                }

                request.comments = args.at(0);
            }
            else if (param == colorIndexParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << colorIndexParam << endl;
                    return false;
                }

                request.colorIndex = std::stoi(args.at(0));
            }
            else if (param == locationParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, MarkerLocation> locationMap = { // ML_Unknown isn't here because it's tech-only
                    RVRS_MAP_ENTRY(MarkerLocation, MLC_MainRuler),
                    RVRS_MAP_ENTRY(MarkerLocation, MLC_Track)
                };

                if (locationMap.count(theArg) == 0)
                {
                    string errorMessage = (theArg == "MLC_Unknown") ? "Don't use this enum value, it's tech-only: "
                                                                    : "There is no such an argument: ";
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationParam << ". "
                         << errorMessage << theArg << endl;
                    return false;
                }

                request.location = locationMap.at(theArg);
            }
            else if (param == trackNameParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackNameParam << endl;
                    return false;
                }

                request.trackName = args.at(0);
            }
            else if (param == venueSnapshotIndex)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << venueSnapshotIndex << endl;
                    return false;
                }

                request.generalProperties.venueSnapshotIndex = std::stoi(args.at(0));
            }
            else if (param == venueSnapshotName)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << venueSnapshotName << endl;
                    return false;
                }

                request.generalProperties.venueSnapshotName = args.at(0);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.EditMemoryLocation(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
