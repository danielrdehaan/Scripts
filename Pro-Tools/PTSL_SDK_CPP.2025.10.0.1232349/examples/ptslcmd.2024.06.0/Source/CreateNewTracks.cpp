// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::CreateNewTracks command.
 */

#include "Common.h"

const string g_pszCreateNewTracks = "CreateNewTracks";
const string g_pszCreateNewTracksHelp =
    "CreateNewTracks [-limit <limit>] [-offset <offset>] [-number_of_tracks <number_of_tracks>] -track_name <track_name> -track_type {TT_Midi|TT_Audio|TT_Aux|TT_Video|TT_Vca|TT_Tempo|TT_Markers|TT_Meter|TT_KeySignature|TT_ChordSymbols|TT_Instrument|TT_Master|TT_Heat|TT_BasicFolder|TT_RoutingFolder|TT_CompLane} [-track_format {TF_Mono|TF_Stereo|TF_LCR|TF_LCRS|TF_Quad|TF_5_0|TF_5_1|TF_5_0_2|TF_5_1_2|TF_5_0_4|TF_5_1_4|TF_6_0|TF_6_1|TF_7_0|TF_7_1|TF_7_0_SDDS|TF_7_1_SDDS|TF_7_0_2|TF_7_1_2|TF_7_0_4|TF_7_1_4|TF_7_0_6|TF_7_1_6|TF_9_0_4|TF_9_1_4|TF_9_0_6|TF_9_1_6|TF_1stOrderAmbisonics|TF_2ndOrderAmbisonics|TF_3rdOrderAmbisonics|TF_4thOrderAmbisonics|TF_5thOrderAmbisonics|TF_6thOrderAmbisonics|TF_7thOrderAmbisonics}] [-track_timebase {TTB_Samples|TTB_Ticks}] [-insertion_point_position {TIPoint_Unknown|TIPoint_Before|TIPoint_After|TIPoint_First|TIPoint_Last}] [-insertion_point_track_name <insertion_point_track_name>]";

PtslCmdCommandResult CreateNewTracks(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CreateNewTracksRequest request;
    request.commandType = CommandType::CreateNewTracks;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.numberOfTracks = 1;

        const std::string pageLimitParam = "limit";
        const std::string offsetParam = "offset";
        const string numberOfTracksParam = "number_of_tracks";
        const string trackNameParam = "track_name";
        const string trackTypeParam = "track_type";
        const string trackFormatParam = "track_format";
        const string trackTimebaseParam = "track_timebase";
        const string insertionPointPosition = "insertion_point_position";
        const string insertionPointTrackName = "insertion_point_track_name";

        bool missedTrackName = true;
        bool missedTrackType = true;

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

            if (pair.first == pageLimitParam)
            {
                if (pair.second.size() != 1)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << pageLimitParam << endl;
                    return false;
                }

                request.paginationRequest.limit = stoi(pair.second.at(0));
            }
            else if (pair.first == offsetParam)
            {
                if (pair.second.size() != 1)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << offsetParam << endl;
                    return false;
                }

                request.paginationRequest.offset = stoi(pair.second[0]);
            }
            else if (param == numberOfTracksParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << numberOfTracksParam << endl;
                    return false;
                }

                request.numberOfTracks = std::stoi(args.at(0));
            }
            else if (param == trackNameParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackNameParam << endl;
                    return false;
                }

                request.trackName = args.at(0);
                missedTrackName = false;
            }
            else if (param == trackTypeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackTypeParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TrackType>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackTypeParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.trackType = theValue.value();
                missedTrackType = false;
            }
            else if (param == trackFormatParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackFormatParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TrackFormat>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackFormatParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.trackFormat = theValue.value();
            }
            else if (param == trackTimebaseParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackTimebaseParam << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TrackTimebase>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackTimebaseParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.trackTimebase = theValue.value();
            }
            else if (param == insertionPointPosition)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << insertionPointPosition << endl;
                    return false;
                }

                const auto theArg = args.at(0);
                const auto theValue = StringToEnum<TrackInsertionPoint>(theArg);

                if (!theValue.has_value())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << insertionPointPosition
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.insertionPointPosition = theValue.value();
            }
            else if (param == insertionPointTrackName)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << insertionPointTrackName << endl;
                    return false;
                }

                request.insertionPointTrackName = args.at(0);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }

        if (missedTrackName || missedTrackType)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (missedTrackName)
            {
                cout << " track_name";
            }

            if (missedTrackType)
            {
                cout << " track_type";
            }

            cout << endl;

            return false;
        }
    }

    const auto rsp = client.CreateNewTracks(request);
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    cout << "CreateNewTracks Response:" << endl;
    cout << "\tcreated tracks: " << rsp->numberOfTracks << endl;
    for (const auto& trackName : rsp->createdTrackNames)
    {
        cout << "\t" << trackName << endl;
    }

    return PtslCmdCommandResult(rsp);
}
