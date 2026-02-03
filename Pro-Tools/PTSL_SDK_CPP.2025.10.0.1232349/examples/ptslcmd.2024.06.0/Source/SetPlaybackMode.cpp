// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetPlaybackMode command.
 */

#include "Common.h"

const std::string g_pszSetPlaybackMode = "SetPlaybackMode";
const std::string g_pszSetPlaybackModeHelp = "SetPlaybackMode -playback_mode {Normal|Loop|DynamicTransport}";

PtslCmdCommandResult SetPlaybackMode(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SetPlaybackModeRequest request;
    request.commandType = CommandType::SetPlaybackMode;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissed = true;

        request.playbackMode = PM_PlaybackMode::PM_Normal;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-playback_mode") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Normal") == 0)
                    {
                        request.playbackMode = PM_PlaybackMode::PM_Normal;
                    }
                    else if (strcmp(params.at(i).c_str(), "Loop") == 0)
                    {
                        request.playbackMode = PM_PlaybackMode::PM_Loop;
                    }
                    else if (strcmp(params.at(i).c_str(), "DynamicTransport") == 0)
                    {
                        request.playbackMode = PM_PlaybackMode::PM_DynamicTransport;
                    }

                    bMissed = false;
                }
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " playback_mode" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetPlaybackMode(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
