// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::Import command.
 */

#include "Common.h"

const std::string g_pszImport = "Import";
const std::string g_pszImportHelp =
    "Import -type {Session|Audio|Video|MIDI|ClipGroups} [-session_path <session_path>] [-file_list <file_list>] [-audio_options {LinkToSource|CopyFromSource|ConsolidateFromSource|ForceToTargetSessionFormat}] [-audio_handle_size <audio_handle_size>] [-video_options {LinkToSource|CopyFromSource}] [-match_options {None|MatchTracks|ImportAsNewTrack}] [-playlist_options {ImportReplaceExistingPlaylists|ImportOverlayNewOnExistingPlaylists|DoNotImport}] [-preset_path <preset_path>] [-no_clip_gain] [-no_clips_and_media] [-no_volume_automation] [-audio_operations {AddAudio|CopyAudio|ConvertAudio|Default}] [-destination_path <destination_path>] [-audio_destination {NewTrack|ClipList}] [-audio_location {SessionStart|SongStart|Selection|Spot}] [-location_type {Start|SyncPoint|End}] [-location_value <location_value>] [-location_options {BarsBeats|MinSecs|TimeCode|FeetFrames|Samples}] [-timecode_mapping_options {MaintainAbsoluteTimeCodeValues|MaintainRelativeTimeCodeValues|MapStartTimeCodeTo}] [-timecode_mapping_start_time <timecode_mapping_start_time>] [-no_adjust_session_start_time_to_match_source]";

PtslCmdCommandResult Import(const std::vector<std::string>& params, CppPTSLClient& client)
{
    ImportRequest request;
    request.commandType = CommandType::Import;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissed = true;

        request.importType = ImportType::Session;
        request.audioOptions = AudioMediaOptions::CopyFromSource;
        request.audioHandleSize = 1000;
        request.videoOptions = VideoMediaOptions::CopyFromSource;
        request.matchOptions = MatchTrackOptions::ImportAsNewTrack;
        request.playlistOptions = MainPlaylistOptions::ImportOverlayNewOnExistingPlaylists;
        request.clipGain = true;
        request.clipsAndMedia = true;
        request.volumeAutomation = true;
        request.timeCodeMappingUnits = TimeCodeMappingOptions::MaintainAbsoluteTimeCodeValues;
        request.timeCodeMappingStartTime = "00:00:00:00";
        request.adjustSessionStartTimeToMatchSource = true;

        size_t count = params.size();

        for (size_t i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Session") == 0)
                    {
                        request.importType = ImportType::Session;
                    }
                    else if (strcmp(params.at(i).c_str(), "Audio") == 0)
                    {
                        request.importType = ImportType::Audio;
                    }

                    bMissed = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-session_path") == 0)
            {
                if (++i < count)
                {
                    request.sessionPath = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_list") == 0)
            {
                auto paramsWithArgs = CommandLineParser::RetrieveParamsWithArgs(params);
                auto paramIter = paramsWithArgs.find("file_list");

                if (paramIter != paramsWithArgs.end())
                {
                    for (const auto& file : paramIter->second)
                    {
                        if (++i < count)
                        {
                            request.audioData.filesList.push_back(file);
                        }
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-audio_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "LinkToSource") == 0)
                    {
                        request.audioOptions = AudioMediaOptions::LinkToSource;
                    }
                    else if (strcmp(params.at(i).c_str(), "CopyFromSource") == 0)
                    {
                        request.audioOptions = AudioMediaOptions::CopyFromSource;
                    }
                    else if (strcmp(params.at(i).c_str(), "ConsolidateFromSource") == 0)
                    {
                        request.audioOptions = AudioMediaOptions::ConsolidateFromSource;
                    }
                    else if (strcmp(params.at(i).c_str(), "ForceToTargetSessionFormat") == 0)
                    {
                        request.audioOptions = AudioMediaOptions::ForceToTargetSessionFormat;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-audio_handle_size") == 0)
            {
                if (++i < count)
                {
                    request.audioHandleSize = atoi(params.at(i).c_str());
                }
            }
            else if (strcmp(params.at(i).c_str(), "-video_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "LinkToSource") == 0)
                    {
                        request.videoOptions = VideoMediaOptions::LinkToSource;
                    }
                    else if (strcmp(params.at(i).c_str(), "CopyFromSource") == 0)
                    {
                        request.videoOptions = VideoMediaOptions::CopyFromSource;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-match_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.matchOptions = MatchTrackOptions::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "MatchTracks") == 0)
                    {
                        request.matchOptions = MatchTrackOptions::MatchTracks;
                    }
                    else if (strcmp(params.at(i).c_str(), "ImportAsNewTrack") == 0)
                    {
                        request.matchOptions = MatchTrackOptions::ImportAsNewTrack;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-playlist_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "ImportReplaceExistingPlaylists") == 0)
                    {
                        request.playlistOptions = MainPlaylistOptions::ImportReplaceExistingPlaylists;
                    }
                    else if (strcmp(params.at(i).c_str(), "ImportOverlayNewOnExistingPlaylists") == 0)
                    {
                        request.playlistOptions = MainPlaylistOptions::ImportOverlayNewOnExistingPlaylists;
                    }
                    else if (strcmp(params.at(i).c_str(), "DoNotImport") == 0)
                    {
                        request.playlistOptions = MainPlaylistOptions::DoNotImport;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-preset_path") == 0)
            {
                if (++i < count)
                {
                    request.trackDataPresetPath = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-no_clip_gain") == 0)
            {
                request.clipGain = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_clips_and_media") == 0)
            {
                request.clipsAndMedia = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_volume_automation") == 0)
            {
                request.volumeAutomation = false;
            }
            else if (strcmp(params.at(i).c_str(), "-audio_operations") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "AddAudio") == 0)
                    {
                        request.audioData.audioOperations = AudioOperations::AddAudio;
                    }
                    else if (strcmp(params.at(i).c_str(), "CopyAudio") == 0)
                    {
                        request.audioData.audioOperations = AudioOperations::CopyAudio;
                    }
                    else if (strcmp(params.at(i).c_str(), "ConvertAudio") == 0)
                    {
                        request.audioData.audioOperations = AudioOperations::ConvertAudio;
                    }
                    else if (strcmp(params.at(i).c_str(), "Default") == 0)
                    {
                        request.audioData.audioOperations = AudioOperations::Default;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-destination_path") == 0)
            {
                if (++i < count)
                {
                    request.audioData.destinationPath = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-audio_destination") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "NewTrack") == 0)
                    {
                        request.audioData.destination = MediaDestination::NewTrack;
                    }
                    else if (strcmp(params.at(i).c_str(), "ClipList") == 0)
                    {
                        request.audioData.destination = MediaDestination::ClipList;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-audio_location") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "SessionStart") == 0)
                    {
                        request.audioData.location = MediaLocation::SessionStart;
                    }
                    else if (strcmp(params.at(i).c_str(), "SongStart") == 0)
                    {
                        request.audioData.location = MediaLocation::SongStart;
                    }
                    else if (strcmp(params.at(i).c_str(), "Selection") == 0)
                    {
                        request.audioData.location = MediaLocation::Selection;
                    }
                    else if (strcmp(params.at(i).c_str(), "Spot") == 0)
                    {
                        request.audioData.location = MediaLocation::Spot;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-timecode_mapping_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "MaintainAbsoluteTimeCodeValues") == 0)
                    {
                        request.timeCodeMappingUnits = TimeCodeMappingOptions::MaintainAbsoluteTimeCodeValues;
                    }
                    else if (strcmp(params.at(i).c_str(), "MaintainRelativeTimeCodeValues") == 0)
                    {
                        request.timeCodeMappingUnits = TimeCodeMappingOptions::MaintainRelativeTimeCodeValues;
                    }
                    else if (strcmp(params.at(i).c_str(), "MapStartTimeCodeTo") == 0)
                    {
                        request.timeCodeMappingUnits = TimeCodeMappingOptions::MapStartTimeCodeTo;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-timecode_mapping_start_time") == 0)
            {
                if (++i < count)
                {
                    request.timeCodeMappingStartTime = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-no_adjust_session_start_time_to_match_source") == 0)
            {
                request.adjustSessionStartTimeToMatchSource = false;
            }
            else if (strcmp(params.at(i).c_str(), "-location_value") == 0)
            {
                if (++i < count)
                {
                    request.spotLocationData.locationValue = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-location_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Start") == 0)
                    {
                        request.spotLocationData.locationType = SpotLocationType::Start;
                    }
                    else if (strcmp(params.at(i).c_str(), "End") == 0)
                    {
                        request.spotLocationData.locationType = SpotLocationType::End;
                    }
                    else if (strcmp(params.at(i).c_str(), "SyncPoint") == 0)
                    {
                        request.spotLocationData.locationType = SpotLocationType::SyncPoint;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-location_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "BarsBeats") == 0)
                    {
                        request.spotLocationData.locationOptions = TrackOffsetOptions::BarsBeats;
                    }
                    else if (strcmp(params.at(i).c_str(), "MinSecs") == 0)
                    {
                        request.spotLocationData.locationOptions = TrackOffsetOptions::MinSecs;
                    }
                    else if (strcmp(params.at(i).c_str(), "TimeCode") == 0)
                    {
                        request.spotLocationData.locationOptions = TrackOffsetOptions::TimeCode;
                    }
                    else if (strcmp(params.at(i).c_str(), "FeetFrames") == 0)
                    {
                        request.spotLocationData.locationOptions = TrackOffsetOptions::FeetFrames;
                    }
                    else if (strcmp(params.at(i).c_str(), "Samples") == 0)
                    {
                        request.spotLocationData.locationOptions = TrackOffsetOptions::Samples;
                    }
                }
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " type" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.Import(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
