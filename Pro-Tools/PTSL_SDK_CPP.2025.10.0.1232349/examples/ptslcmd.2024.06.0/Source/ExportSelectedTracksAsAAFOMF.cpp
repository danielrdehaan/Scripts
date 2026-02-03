// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ExportSelectedTracksAsAAFOMF command.
 */

#include "Common.h"

const std::string g_pszExportSelectedTracksAsAAFOMF = "ExportSelectedTracksAsAAFOMF";
const std::string g_pszExportSelectedTracksAsAAFOMFHelp =
    "ExportSelectedTracksAsAAFOMF -container_file_name <container_file_name> -container_file_location <container_file_location> -asset_file_location <asset_file_location> -sequence_name <sequence_name> [-file_type {WAV|AIFF|MXF|Embedded}] [-bit_depth {Bit16|Bit24}] [-copy_option {ConsolidateFromSource|CopyFromSource|LinkFromSource}] [-no_compatibility] [-no_quantize_edits] [-no_multichannel] [-comments <comments>]";

PtslCmdCommandResult ExportSelectedTracksAsAAFOMF(const std::vector<std::string>& params, CppPTSLClient& client)
{
    ExportSelectedTracksAsAAFOMFRequest request;
    request.commandType = CommandType::ExportSelectedTracksAsAAFOMF;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissedContainerFileName = true;
        bool bMissedContainerFileLocation = true;
        bool bMissedAssetFileLocation = true;
        bool bMissedSequenceName = true;

        request.fileType = ExportAsAAFFileType::WAV;
        request.bitDepth = AAFFileBitDepth::Bit24;
        request.copyOption = CopyOption::CopyFromSourceMedia;
        request.enforceMediaComposerCompatibility = true;
        request.quantizeEditsToFrameBoundaries = true;
        request.exportStereoAsMultichannel = true;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-container_file_name") == 0)
            {
                if (++i < count)
                {
                    request.containerFileName = params.at(i);
                    bMissedContainerFileName = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-container_file_location") == 0)
            {
                if (++i < count)
                {
                    request.containerFileLocation = params.at(i);
                    bMissedContainerFileLocation = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-asset_file_location") == 0)
            {
                if (++i < count)
                {
                    request.assetFileLocation = params.at(i);
                    bMissedAssetFileLocation = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-sequence_name") == 0)
            {
                if (++i < count)
                {
                    request.sequenceName = params.at(i);
                    bMissedSequenceName = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "WAV") == 0)
                    {
                        request.fileType = ExportAsAAFFileType::WAV;
                    }
                    else if (strcmp(params.at(i).c_str(), "AIFF") == 0)
                    {
                        request.fileType = ExportAsAAFFileType::AIFF;
                    }
                    else if (strcmp(params.at(i).c_str(), "MXF") == 0)
                    {
                        request.fileType = ExportAsAAFFileType::MXF;
                    }
                    else if (strcmp(params.at(i).c_str(), "Embedded") == 0)
                    {
                        request.fileType = ExportAsAAFFileType::Embedded;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-bit_depth") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Bit16") == 0)
                    {
                        request.bitDepth = AAFFileBitDepth::Bit16;
                    }
                    else if (strcmp(params.at(i).c_str(), "Bit24") == 0)
                    {
                        request.bitDepth = AAFFileBitDepth::Bit24;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-copy_option") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "ConsolidateFromSource") == 0)
                    {
                        request.copyOption = CopyOption::ConsolidateFromSourceMedia;
                    }
                    else if (strcmp(params.at(i).c_str(), "CopyFromSource") == 0)
                    {
                        request.copyOption = CopyOption::CopyFromSourceMedia;
                    }
                    else if (strcmp(params.at(i).c_str(), "LinkFromSource") == 0)
                    {
                        request.copyOption = CopyOption::LinkFromSourceMedia;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-no_compatibility") == 0)
            {
                request.enforceMediaComposerCompatibility = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_quantize_edits") == 0)
            {
                request.quantizeEditsToFrameBoundaries = false;
            }
            else if (strcmp(params.at(i).c_str(), "-no_multichannel") == 0)
            {
                request.exportStereoAsMultichannel = false;
            }
            else if (strcmp(params.at(i).c_str(), "-comments") == 0)
            {
                if (++i < count)
                {
                    request.comments = params.at(i);
                }
            }
        }

        if (bMissedContainerFileName || bMissedContainerFileLocation || bMissedAssetFileLocation || bMissedSequenceName)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (bMissedContainerFileName)
            {
                cout << " container_file_name";
            }
            if (bMissedContainerFileLocation)
            {
                cout << " container_file_location";
            }
            if (bMissedAssetFileLocation)
            {
                cout << " asset_file_location";
            }
            if (bMissedSequenceName)
            {
                cout << " sequence_name";
            }

            cout << endl;

            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.ExportSelectedTracksAsAAFOMF(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
