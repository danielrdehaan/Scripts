// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::CreateSession command.
 */

#include "Common.h"

const std::string g_pszCreateSession = "CreateSession";
const std::string g_pszCreateSessionHelp =
    "CreateSession -document_name <document_name> -folder_location <folder_location> [-from_template -template_category <template_category> -template_name <template_name>] [-file_type {WAVE|AIFF" /*|AAF|OMF*/
    "}] [-sample_rate {44100|48000|88200|96000|176400|192000}] [-I/O_settings {Last|StereoMix|51FilmMix|51SMPTEMix|51DTSMix|UserDefined}] [-interleaved] [-project]" /*[-from_aaf -path_to_aaf <path_to_aaf>]*/
    " [-bit_depth {Bit16|Bit24|Bit32Float}]";

PtslCmdCommandResult CreateSession(const std::vector<std::string>& params, CppPTSLClient& client)
{
    bool bMissedDocumentName = true;
    bool bMissedFolderLocation = true;

    CreateSessionRequest request;
    request.commandType = CommandType::CreateSession;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.createFromTemplate = false;
        request.fileType = FileType::WAVE;
        request.sampleRate = SampleRate::SR_44100;
        request.inputOutputSettings = IOSettings::IO_StereoMix;
        request.isInterleaved = false;
        request.isCloudProject = false;
        request.createFromAAF = false;
        request.bitDepth = BitDepth::Bit16;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-document_name") == 0)
            {
                if (++i < count)
                {
                    request.sessionName = params.at(i);
                    bMissedDocumentName = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-folder_location") == 0)
            {
                if (++i < count)
                {
                    request.sessionLocation = params.at(i);
                    bMissedFolderLocation = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-from_template") == 0)
            {
                request.createFromTemplate = true;
            }
            else if (strcmp(params.at(i).c_str(), "-template_category") == 0)
            {
                if (++i < count)
                {
                    request.templateGroup = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-template_name") == 0)
            {
                if (++i < count)
                {
                    request.templateName = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-from_aaf") == 0)
            {
                request.createFromAAF = true;
            }
            else if (strcmp(params.at(i).c_str(), "-path_to_aaf") == 0)
            {
                if (++i < count)
                {
                    request.pathToAAF = params.at(i);
                }
            }
            else if (strcmp(params.at(i).c_str(), "-file_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "WAVE") == 0)
                    {
                        request.fileType = FileType::WAVE;
                    }
                    else if (strcmp(params.at(i).c_str(), "AIFF") == 0)
                    {
                        request.fileType = FileType::AIFF;
                    }
                    else if (strcmp(params.at(i).c_str(), "AAF") == 0)
                    {
                        request.fileType = FileType::AAF;
                    }
                    else if (strcmp(params.at(i).c_str(), "OMF") == 0)
                    {
                        request.fileType = FileType::OMF;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-sample_rate") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "None") == 0)
                    {
                        request.sampleRate = SampleRate::SR_None;
                    }
                    else if (strcmp(params.at(i).c_str(), "44100") == 0)
                    {
                        request.sampleRate = SampleRate::SR_44100;
                    }
                    else if (strcmp(params.at(i).c_str(), "48000") == 0)
                    {
                        request.sampleRate = SampleRate::SR_48000;
                    }
                    else if (strcmp(params.at(i).c_str(), "88200") == 0)
                    {
                        request.sampleRate = SampleRate::SR_88200;
                    }
                    else if (strcmp(params.at(i).c_str(), "96000") == 0)
                    {
                        request.sampleRate = SampleRate::SR_96000;
                    }
                    else if (strcmp(params.at(i).c_str(), "176400") == 0)
                    {
                        request.sampleRate = SampleRate::SR_176400;
                    }
                    else if (strcmp(params.at(i).c_str(), "192000") == 0)
                    {
                        request.sampleRate = SampleRate::SR_192000;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-I/O_settings") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Last") == 0)
                    {
                        request.inputOutputSettings = IOSettings::IO_Last;
                    }
                    else if (strcmp(params.at(i).c_str(), "StereoMix") == 0)
                    {
                        request.inputOutputSettings = IOSettings::IO_StereoMix;
                    }
                    else if (strcmp(params.at(i).c_str(), "51FilmMix") == 0)
                    {
                        request.inputOutputSettings = IOSettings::IO_51FilmMix;
                    }
                    else if (strcmp(params.at(i).c_str(), "51SMPTEMix") == 0)
                    {
                        request.inputOutputSettings = IOSettings::IO_51SMPTEMix;
                    }
                    else if (strcmp(params.at(i).c_str(), "51DTSMix") == 0)
                    {
                        request.inputOutputSettings = IOSettings::IO_51DTSMix;
                    }
                    else if (strcmp(params.at(i).c_str(), "UserDefined") == 0)
                    {
                        request.inputOutputSettings = IOSettings::IO_UserDefined;
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
            else if (strcmp(params.at(i).c_str(), "-interleaved") == 0)
            {
                request.isInterleaved = true;
            }
            else if (strcmp(params.at(i).c_str(), "-project") == 0)
            {
                request.isCloudProject = true;
            }
        }

        if (bMissedDocumentName || bMissedFolderLocation)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (bMissedDocumentName)
            {
                cout << " document_name";
            }

            if (bMissedFolderLocation)
            {
                cout << " folder_location";
            }

            cout << endl;

            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.CreateSession(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
