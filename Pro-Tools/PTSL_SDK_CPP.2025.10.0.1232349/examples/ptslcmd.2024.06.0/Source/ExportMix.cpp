// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ExportMix command.
 */

#include "Common.h"

const std::string g_pszExportMix = "ExportMix";
const std::string g_pszExportMixHelp =
    "ExportMix [-preset_path <preset_path>] [-file_name <file_name>] [-file_type {None|WAV|AIFF|MP3|MXFOPAtom|WAVADM|MOV}] [-mix_source_list <\"[{source_type:\\\"<PhysicalOut|Bus|Output>\\\",name:\\\"<name (with channels)>\\\"},{...}...]\">] [-compression_type {None|PCM}] [-export_format {None|Mono|MultipleMono|Interleaved}] [-bit_depth {None|Bit16|Bit24|Bit32Float}] [-sample_rate {None|SR_44100|SR_48000|SR_88200|SR_96000|SR_176400|SR_192000}] [-delivery_format {None|FilePerMixSource|SingleFile}] [-pad_to_frame_boundary {None|False|True}] [-add_first_frame_of_action {None|False|True}] [-timecode_value <timecode_value>] [-frame_rate <frame_rate>] [-dolby_property_list <\"[{name:\\\"<name>\\\",type:\\\"<DP_VT_double|DP_VT_float|DP_VT_int32|DP_VT_int64|DP_VT_uint32|DP_VT_uint64|DP_VT_sint32|DP_VT_sint64|DP_VT_fixed32|DP_VT_fixed64|DP_VT_sfixed32|DP_VT_sfixed64|DP_VT_bool|DP_VT_string|DP_VT_bytes|DP_VT_enum|DP_VT_object>\\\",value:\\\"<value>\\\"},{...}...]\">] [-include_video {None|False|True}] [-video_export_options {None|SameAsSource|Transcode}] [-replace_timecode_track {None|False|True}] [-codec_name <codec_name>] [-codec_property_list <\"[{name:\\\"<name>\\\",type:\\\"<DP_VT_double|DP_VT_float|DP_VT_int32|DP_VT_int64|DP_VT_uint32|DP_VT_uint64|DP_VT_sint32|DP_VT_sint64|DP_VT_fixed32|DP_VT_fixed64|DP_VT_sfixed32|DP_VT_sfixed64|DP_VT_bool|DP_VT_string|DP_VT_bytes|DP_VT_enum|DP_VT_object>\\\",value:\\\"<value>\\\"},{...}...]\">] [-import_after_bounce {None|False|True}] [-import_destination {None|MainVideoTrack|NewTrack|ClipList}] [-import_location {None|SessionStart|SongStart|Selection|Spot}] [-gaps_between_clips <gaps_between_clips>] [-import_audio_from_file {None|False|True}] [-remove_existing_video_tracks {None|False|True}] [-remove_existing_video_clips {None|False|True}] [-clear_destination_video_track_playlist {None|False|True}] [-file_destination {None|SessionFolder|Directory}] [-directory <directory>] [-offline_bounce {None|False|True}] [-sample_rate_custom <sample_rate_custom>]";

PtslCmdCommandResult ExportMix(const std::vector<std::string>& params, CppPTSLClient& client)
{
    ExportMixRequest request = {};
    request.commandType = CommandType::ExportMix;
    request.audioInfo.sampleRateCustom = 0;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-preset_path") == 0)
            {
                if (++i < count)
                {
                    request.presetPath = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_name") == 0)
            {
                if (++i < count)
                {
                    request.fileName = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.fileType = EM_FileType::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "MOV") == 0)
                    {
                        request.fileType = EM_FileType::MOV;
                    }
                    else if (strcmp(params.at(i).c_str(), "WAV") == 0)
                    {
                        request.fileType = EM_FileType::WAV;
                    }
                    else if (strcmp(params.at(i).c_str(), "AIFF") == 0)
                    {
                        request.fileType = EM_FileType::AIFF;
                    }
                    else if (strcmp(params.at(i).c_str(), "MP3") == 0)
                    {
                        request.fileType = EM_FileType::MP3;
                    }
                    else if (strcmp(params.at(i).c_str(), "MXFOPAtom") == 0)
                    {
                        request.fileType = EM_FileType::MXFOPAtom;
                    }
                    else if (strcmp(params.at(i).c_str(), "WAVADM") == 0)
                    {
                        request.fileType = EM_FileType::WAVADM;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-files_list") == 0)
            {
                if (++i < count)
                {
                    request.filesList = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-mix_source_list") == 0)
            {
                if (++i < count)
                {
                    request.mixSourceList = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-compression_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.audioInfo.compressionType = CompressionType::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "PCM") == 0)
                    {
                        request.audioInfo.compressionType = CompressionType::PCM;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-export_format") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.audioInfo.exportFormat = ExportFormat::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "Mono") == 0)
                    {
                        request.audioInfo.exportFormat = ExportFormat::Mono;
                    }
                    else if (strcmp(params.at(i).c_str(), "MultipleMono") == 0)
                    {
                        request.audioInfo.exportFormat = ExportFormat::MultipleMono;
                    }
                    else if (strcmp(params.at(i).c_str(), "Interleaved") == 0)
                    {
                        request.audioInfo.exportFormat = ExportFormat::Interleaved;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-bit_depth") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.audioInfo.bitDepth = BitDepth::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "Bit16") == 0)
                    {
                        request.audioInfo.bitDepth = BitDepth::Bit16;
                    }
                    else if (strcmp(params.at(i).c_str(), "Bit24") == 0)
                    {
                        request.audioInfo.bitDepth = BitDepth::Bit24;
                    }
                    else if (strcmp(params.at(i).c_str(), "Bit32Float") == 0)
                    {
                        request.audioInfo.bitDepth = BitDepth::Bit32Float;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-sample_rate") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_None;
                    }
                    else if (strcmp(params.at(i).c_str(), "SR_44100") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_44100;
                    }
                    else if (strcmp(params.at(i).c_str(), "SR_48000") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_48000;
                    }
                    else if (strcmp(params.at(i).c_str(), "SR_88200") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_88200;
                    }
                    else if (strcmp(params.at(i).c_str(), "SR_96000") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_96000;
                    }
                    else if (strcmp(params.at(i).c_str(), "SR_176400") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_176400;
                    }
                    else if (strcmp(params.at(i).c_str(), "SR_192000") == 0)
                    {
                        request.audioInfo.sampleRate = SampleRate::SR_192000;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-delivery_format") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.audioInfo.deliveryFormat = EM_DeliveryFormat::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "FilePerMixSource") == 0)
                    {
                        request.audioInfo.deliveryFormat = EM_DeliveryFormat::FilePerMixSource;
                    }
                    else if (strcmp(params.at(i).c_str(), "SingleFile") == 0)
                    {
                        request.audioInfo.deliveryFormat = EM_DeliveryFormat::SingleFile;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-pad_to_frame_boundary") == 0)
            {
                if (++i < count)
                {
                    request.audioInfo.padToFrameBoundary =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-sample_rate_custom") == 0)
            {
                if (++i < count)
                {
                    request.audioInfo.sampleRateCustom = stoi(params.at(i).c_str());
                }
            }
            else if (strcmp(params.at(i).c_str(), "-add_first_frame_of_action") == 0)
            {
                if (++i < count)
                {
                    request.dolbyAtmosInfo.firstFrameOfAction =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-timecode_value") == 0)
            {
                if (++i < count)
                {
                    request.dolbyAtmosInfo.timeCodeValue = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-frame_rate") == 0)
            {
                if (++i < count)
                {
                    request.dolbyAtmosInfo.frameRate = atoi(params.at(i).c_str());
                }
            }
            else if (strcmp(params.at(i).c_str(), "-dolby_property_list") == 0)
            {
                if (++i < count)
                {
                    request.dolbyAtmosInfo.propertyList = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-include_video") == 0)
            {
                if (++i < count)
                {
                    request.videoInfo.includeVideo =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-video_export_options") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.videoInfo.videoExportOptions = EM_VideoExportOptions::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "SameAsSource") == 0)
                    {
                        request.videoInfo.videoExportOptions = EM_VideoExportOptions::SameAsSource;
                    }
                    else if (strcmp(params.at(i).c_str(), "Transcode") == 0)
                    {
                        request.videoInfo.videoExportOptions = EM_VideoExportOptions::Transcode;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-replace_timecode_track") == 0)
            {
                if (++i < count)
                {
                    request.videoInfo.replaceTimeCodeTrack =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-codec_name") == 0)
            {
                if (++i < count)
                {
                    request.videoInfo.codecInfo.codecName = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-codec_property_list") == 0)
            {
                if (++i < count)
                {
                    request.videoInfo.codecInfo.propertyList = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-import_after_bounce") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.importAfterBounce =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-import_destination") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.locationInfo.importOptions.importDestination = MediaDestination::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "MainVideoTrack") == 0)
                    {
                        request.locationInfo.importOptions.importDestination = MediaDestination::MainVideoTrack;
                    }
                    else if (strcmp(params.at(i).c_str(), "NewTrack") == 0)
                    {
                        request.locationInfo.importOptions.importDestination = MediaDestination::NewTrack;
                    }
                    else if (strcmp(params.at(i).c_str(), "ClipList") == 0)
                    {
                        request.locationInfo.importOptions.importDestination = MediaDestination::ClipList;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-import_location") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.locationInfo.importOptions.importLocation = MediaLocation::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "SessionStart") == 0)
                    {
                        request.locationInfo.importOptions.importLocation = MediaLocation::SessionStart;
                    }
                    else if (strcmp(params.at(i).c_str(), "SongStart") == 0)
                    {
                        request.locationInfo.importOptions.importLocation = MediaLocation::SongStart;
                    }
                    else if (strcmp(params.at(i).c_str(), "Selection") == 0)
                    {
                        request.locationInfo.importOptions.importLocation = MediaLocation::Selection;
                    }
                    else if (strcmp(params.at(i).c_str(), "Spot") == 0)
                    {
                        request.locationInfo.importOptions.importLocation = MediaLocation::Spot;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-gaps_between_clips") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.importOptions.gapsBetweenClips = atoi(params.at(i).c_str());
                }
            }
            else if (strcmp(params.at(i).c_str(), "-import_audio_from_file") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.importOptions.importAudioFromFile =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-remove_existing_video_tracks") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.importOptions.removeExistingVideoTracks =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-remove_existing_video_clips") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.importOptions.removeExistingVideoClips =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-clear_destination_video_track_playlist") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.importOptions.clearDestinationVideoTrackPlaylist =
                        StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_destination") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.locationInfo.fileDestination = FileDestination::None;
                    }
                    else if (strcmp(params.at(i).c_str(), "SessionFolder") == 0)
                    {
                        request.locationInfo.fileDestination = FileDestination::SessionFolder;
                    }
                    else if (strcmp(params.at(i).c_str(), "Directory") == 0)
                    {
                        request.locationInfo.fileDestination = FileDestination::Directory;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-directory") == 0)
            {
                if (++i < count)
                {
                    request.locationInfo.directory = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-offline_bounce") == 0)
            {
                if (++i < count)
                {
                    request.offlineBounce = StringToEnum<TripleBool>(params.at(i)).value_or(TripleBool::TB_None);
                }
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.ExportMix(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
