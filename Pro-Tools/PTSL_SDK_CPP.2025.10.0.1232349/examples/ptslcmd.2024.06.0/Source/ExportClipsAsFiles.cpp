// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ExportClipsAsFiles command.
 */

#include "Common.h"

const std::string g_pszExportClipsAsFiles = "ExportClipsAsFiles";
const std::string g_pszExportClipsAsFilesHelp =
    "ExportClipsAsFiles -path <path> [-format {MultipleMono|Interleaved}] [-file_type {WAV|AIFF|MXF}] [-bit_depth {Bit16|Bit24|Bit32Float}] [-resolve_names {AutoRenaming|ReplacingWithNewFiles}] [-compatibility] [-sample_rate_custom]";

PtslCmdCommandResult ExportClipsAsFiles(const std::vector<std::string>& params, CppPTSLClient& client)
{
    bool bMissed = true;

    ExportClipsAsFilesRequest request;
    request.commandType = CommandType::ExportClipsAsFiles;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.format = ExportFormat::Interleaved;
        request.fileType = ExportFileType::AIFF;
        request.bitDepth = BitDepth::Bit32Float;
        request.duplicateNames = ResolveDuplicateNamesBy::ReplacingWithNewFiles;
        request.enforceAvidCompatibility = false;
        request.sampleRateCustom = 0;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-path") == 0)
            {
                if (++i < count)
                {
                    request.filePath = params.at(i);
                    bMissed = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-format") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "MultipleMono") == 0)
                    {
                        request.format = ExportFormat::MultipleMono;
                    }
                    else if (strcmp(params.at(i).c_str(), "Interleaved") == 0)
                    {
                        request.format = ExportFormat::Interleaved;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "WAV") == 0)
                    {
                        request.fileType = ExportFileType::WAV;
                    }
                    else if (strcmp(params.at(i).c_str(), "AIFF") == 0)
                    {
                        request.fileType = ExportFileType::AIFF;
                    }
                    else if (strcmp(params.at(i).c_str(), "MXF") == 0)
                    {
                        request.fileType = ExportFileType::MXF;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-bit_depth") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Bit16") == 0)
                    {
                        request.bitDepth = BitDepth::Bit16;
                    }
                    else if (strcmp(params.at(i).c_str(), "Bit24") == 0)
                    {
                        request.bitDepth = BitDepth::Bit24;
                    }
                    else if (strcmp(params.at(i).c_str(), "Bit32Float") == 0)
                    {
                        request.bitDepth = BitDepth::Bit32Float;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-resolve_names") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "AutoRenaming") == 0)
                    {
                        request.duplicateNames = ResolveDuplicateNamesBy::AutoRenaming;
                    }
                    else if (strcmp(params.at(i).c_str(), "ReplacingWithNewFiles") == 0)
                    {
                        request.duplicateNames = ResolveDuplicateNamesBy::ReplacingWithNewFiles;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-compatibility") == 0)
            {
                request.enforceAvidCompatibility = true;
            }
            else if (strcmp(params.at(i).c_str(), "-sample_rate_custom") == 0)
            {
                if (++i < count)
                {
                    request.sampleRateCustom = stoi(params.at(i).c_str());
                }
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " path" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.ExportClipsAsFiles(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
