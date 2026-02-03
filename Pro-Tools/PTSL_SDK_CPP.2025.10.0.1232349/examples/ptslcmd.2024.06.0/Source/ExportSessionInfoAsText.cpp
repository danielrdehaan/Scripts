// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ExportSessionInfoAsText command.
 */

#include "Common.h"

const std::string g_pszExportSessionInfoAsText = "ExportSessionInfoAsText";
const std::string g_pszExportSessionInfoAsTextHelp =
    "ExportSessionInfoAsText [-no_include_file_list] [-no_include_clip_list] [-no_include_markers] [-no_include_plugin_list] [-no_include_track_EDLs] [-show_sub_frames] [-include_user_timestamps] [-track_list_type {AllTracks|SelectedTracksOnly}] [-fade_handling_type {ShowCrossfades|DontShowCrossfades|CombineCrossfadedClips}] [-track_offset_options {BarsBeats|MinSecs|TimeCode|FeetFrames|Samples}] [-file_format {TextEdit|UTF8}] [-output_type {File|String}] [-output_path <output_path>(required parameter if output_type is File)]";

PtslCmdCommandResult ExportSessionInfoAsText(const std::vector<std::string>& params, CppPTSLClient& client)
{
    ExportSessionInfoAsTextRequest request;
    request.commandType = CommandType::ExportSessionInfoAsText;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.includeFileList = true;
        request.includeClipList = true;
        request.includeMarkers = true;
        request.includePluginList = true;
        request.includeTrackEDLs = true;
        request.showSubFrames = false;
        request.includeUserTimestamps = false;
        request.trackListType = TrackListType::AllTracks;
        request.fadeHandlingType = FadeHandlingType::ShowCrossfades;
        request.trackOffsetOptions = TrackOffsetOptions::BarsBeats;
        request.textAsFileFormat = TextAsFileFormat::TextEdit;
        request.outputType = ESI_OutputType::ESI_Unknown;
        request.outputPath = "";

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-no_include_file_list") == 0)
            {
                request.includeFileList = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_include_clip_list") == 0)
            {
                request.includeClipList = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_include_markers") == 0)
            {
                request.includeMarkers = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_include_plugin_list") == 0)
            {
                request.includePluginList = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_include_track_EDLs") == 0)
            {
                request.includeTrackEDLs = false;
            }
            else if (strcmp(params.at(i).c_str(), "-show_sub_frames") == 0)
            {
                request.showSubFrames = true;
            }
            else if (strcmp(params.at(i).c_str(), "-include_user_timestamps") == 0)
            {
                request.includeUserTimestamps = true;
            }
            else if (strcmp(params.at(i).c_str(), "-track_list_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "AllTracks") == 0)
                    {
                        request.trackListType = TrackListType::AllTracks;
                    }
                    else if (strcmp(params.at(i).c_str(), "SelectedTracksOnly") == 0)
                    {
                        request.trackListType = TrackListType::SelectedTracksOnly;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-fade_handling_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "ShowCrossfades") == 0)
                    {
                        request.fadeHandlingType = FadeHandlingType::ShowCrossfades;
                    }
                    else if (strcmp(params.at(i).c_str(), "DontShowCrossfades") == 0)
                    {
                        request.fadeHandlingType = FadeHandlingType::DontShowCrossfades;
                    }
                    else if (strcmp(params.at(i).c_str(), "CombineCrossfadedClips") == 0)
                    {
                        request.fadeHandlingType = FadeHandlingType::CombineCrossfadedClips;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-track_offset_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "BarsBeats") == 0)
                    {
                        request.trackOffsetOptions = TrackOffsetOptions::BarsBeats;
                    }
                    else if (strcmp(params.at(i).c_str(), "MinSecs") == 0)
                    {
                        request.trackOffsetOptions = TrackOffsetOptions::MinSecs;
                    }
                    else if (strcmp(params.at(i).c_str(), "TimeCode") == 0)
                    {
                        request.trackOffsetOptions = TrackOffsetOptions::TimeCode;
                    }
                    else if (strcmp(params.at(i).c_str(), "FeetFrames") == 0)
                    {
                        request.trackOffsetOptions = TrackOffsetOptions::FeetFrames;
                    }
                    else if (strcmp(params.at(i).c_str(), "Samples") == 0)
                    {
                        request.trackOffsetOptions = TrackOffsetOptions::Samples;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_format") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "TextEdit") == 0)
                    {
                        request.textAsFileFormat = TextAsFileFormat::TextEdit;
                    }
                    else if (strcmp(params.at(i).c_str(), "UTF8") == 0)
                    {
                        request.textAsFileFormat = TextAsFileFormat::UTF8;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-output_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "File") == 0)
                    {
                        request.outputType = ESI_OutputType::ESI_File;
                    }
                    else if (strcmp(params.at(i).c_str(), "String") == 0)
                    {
                        request.outputType = ESI_OutputType::ESI_String;
                    }
                    else
                    {
                        request.outputType = ESI_OutputType::ESI_Unknown;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-output_path") == 0)
            {
                if (++i < count)
                {
                    request.outputPath = params.at(i);
                }
            }
        }
    }

    std::shared_ptr<ExportSessionInfoAsTextResponse> rsp = client.ExportSessionInfoAsText(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "ExportSessionInfoAsText Response:" << endl;

        if (!rsp->sessionInfo.empty())
        {
            cout << rsp->sessionInfo << endl;
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "ExportSessionInfoAsText Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::FailedWithBadErrorResponse)
    {
        cout << "ExportSessionInfoAsText failed but returned an incorrect error response" << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::CompletedWithBadResponse)
    {
        cout << "ExportSessionInfoAsText completed but returned an incorrect response" << endl;
    }
    else
    {
        cout << "ExportSessionInfoAsText returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
