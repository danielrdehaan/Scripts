// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ImportVideo command.
 */

#include "Common.h"

const string g_pszImportVideo = "ImportVideo";
const string g_pszImportVideoHelp =
    "ImportVideo -video_file_list <video_file_list> [-destination {None|MainVideoTrack|NewTrack|ClipList}] [-location {None|SessionStart|SongStart|Selection|Spot}] [-import_audio_from_file <import_audio_from_file>] [-audio_destination_path <audioDestinationPath>] [-location_options {BarsBeats|MinSecs|TimeCode|FeetFrames|Samples}] [-location_type {Start|SyncPoint|End}] [-location_value <locationValue>] [-gaps_between_clips <gapsBetweenClips>] [-remove_existing_video_tracks <remove_existing_video_tracks>] [-remove_existing_video_clips <remove_existing_video_clips>] [-clear_destination_video_track_playlist <clear_destination_video_track_playlist>]";

PtslCmdCommandResult ImportVideo(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    ImportVideoRequest request;
    request.commandType = CommandType::ImportVideo;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool missedParam = true;

        request.destination = MediaDestination::NewTrack;
        request.location = MediaLocation::SessionStart;
        request.spotLocationData.locationOptions = TrackOffsetOptions::BarsBeats;
        request.spotLocationData.locationType = SpotLocationType::Start;
        request.spotLocationData.locationValue;
        request.gapsBetweenClips = 0;
        request.importAudioFromFile = true;
        request.audioDestinationPath = "";
        request.removeExistingVideoTracks = false;
        request.removeExistingVideoClips = false;
        request.clearDestinationVideoTrackPlaylist = false;

        const string videoFilesListParam = "video_file_list";
        const string destinationParam = "destination";
        const string locationParam = "location";
        const string importAudioFromFileParam = "import_audio_from_file";
        const string audioDestinationPathParam = "audio_destination_path";
        const string locationOptionsParam = "location_options";
        const string locationTypeParam = "location_type";
        const string locationValueParam = "location_value";
        const string gapsBetweenClipsParam = "gaps_between_clips";
        const string removeExistingVideoTracksParam = "remove_existing_video_tracks";
        const string removeExistingVideoClipsParam = "remove_existing_video_clips";
        const string clearDestinationVideoTrackPlaylistParam = "clear_destination_video_track_playlist";

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

            if (param == videoFilesListParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << videoFilesListParam << endl;
                    return false;
                }

                auto fileListParamIter = paramsArgsMap.find(videoFilesListParam);

                if (fileListParamIter != paramsArgsMap.end())
                {
                    for (const auto& file : fileListParamIter->second)
                    {
                        request.videoFilesList.push_back(file);
                    }
                }

                missedParam = false;
            }
            else if (param == destinationParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << destinationParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, MediaDestination> destinationMap = { // we don't have Unknown value for this enum
                    RVRS_MAP_ENTRY(MediaDestination, None),
                    RVRS_MAP_ENTRY(MediaDestination, MainVideoTrack),
                    RVRS_MAP_ENTRY(MediaDestination, NewTrack),
                    RVRS_MAP_ENTRY(MediaDestination, ClipList)
                };

                if (destinationMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << destinationParam << ". "
                         << "There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.destination = destinationMap.at(theArg);
            }
            else if (param == locationParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, MediaLocation> locationMap = { // we don't have Unknown value for this enum
                    RVRS_MAP_ENTRY(MediaLocation, None),
                    RVRS_MAP_ENTRY(MediaLocation, SessionStart),
                    RVRS_MAP_ENTRY(MediaLocation, SongStart),
                    RVRS_MAP_ENTRY(MediaLocation, Selection),
                    RVRS_MAP_ENTRY(MediaLocation, Spot)
                };

                if (locationMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationParam << ". "
                         << "There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.location = locationMap.at(theArg);
            }
            else if (param == importAudioFromFileParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << importAudioFromFileParam << endl;
                    return false;
                }

                request.importAudioFromFile = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == audioDestinationPathParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << audioDestinationPathParam << endl;
                    return false;
                }

                request.audioDestinationPath = args.at(0);
            }
            else if (param == locationOptionsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationOptionsParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, TrackOffsetOptions> locationOptionsMap = { // we don't have Unknown value for this enum
                    RVRS_MAP_ENTRY(TrackOffsetOptions, BarsBeats),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, MinSecs),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, TimeCode),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, FeetFrames),
                    RVRS_MAP_ENTRY(TrackOffsetOptions, Samples)
                };

                if (locationOptionsMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationOptionsParam << ". "
                         << "There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.spotLocationData.locationOptions = locationOptionsMap.at(theArg);
            }
            else if (param == locationTypeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationTypeParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, SpotLocationType> locationTypeMap = { // we don't have Unknown value for this enum
                    RVRS_MAP_ENTRY(SpotLocationType, Start),
                    RVRS_MAP_ENTRY(SpotLocationType, SyncPoint),
                    RVRS_MAP_ENTRY(SpotLocationType, End)
                };

                if (locationTypeMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationTypeParam << ". "
                         << "There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.spotLocationData.locationType = locationTypeMap.at(theArg);
            }
            else if (param == locationValueParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << locationValueParam << endl;
                    return false;
                }

                request.spotLocationData.locationValue = args.at(0);
            }
            else if (param == gapsBetweenClipsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << gapsBetweenClipsParam << endl;
                    return false;
                }

                request.gapsBetweenClips = static_cast<int32_t>(atoi(args.at(0).c_str()));
            }
            else if (param == removeExistingVideoTracksParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << removeExistingVideoTracksParam
                         << endl;
                    return false;
                }

                request.removeExistingVideoTracks = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == removeExistingVideoClipsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << removeExistingVideoClipsParam
                         << endl;
                    return false;
                }

                request.removeExistingVideoClips = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == clearDestinationVideoTrackPlaylistParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " "
                         << clearDestinationVideoTrackPlaylistParam << endl;
                    return false;
                }

                request.clearDestinationVideoTrackPlaylist = CommandLineParser::StringToBool(args.at(0));
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }

        // Check if all required parameters are filled
        if (missedParam)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " " << videoFilesListParam << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<ImportVideoResponse> rsp = client.ImportVideo(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "ImportVideo Response:" << endl;

        if (rsp->failureList.size())
        {
            cout << "\t"
                 << "failure list:" << endl;
            for (const auto& failure : rsp->failureList)
            {
                cout << "\t" << failure.failureMessage << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "ImportVideo Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "ImportVideo returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
