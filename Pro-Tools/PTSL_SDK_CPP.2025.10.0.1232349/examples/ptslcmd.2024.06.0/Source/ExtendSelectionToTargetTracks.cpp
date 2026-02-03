// Copyright 2020, 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ExtendSelectionToTargetTracks command.
 */

#include "Common.h"

const std::string g_pszExtendSelectionToTargetTracks = "ExtendSelectionToTargetTracks";
const std::string g_pszExtendSelectionToTargetTracksHelp =
    "ExtendSelectionToTargetTracks -track_id_list <track_id_list>";

PtslCmdCommandResult ExtendSelectionToTargetTracks(const std::vector<std::string>& params, CppPTSLClient& client)
{
    ExtendSelectionToTargetTracksRequest request;
    request.commandType = CommandType::ExtendSelectionToTargetTracks;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        size_t count = params.size();

        if (count > 1 && strcmp(params.at(0).c_str(), "-track_id_list") == 0)
        {

            for (int i = 1; i < count; ++i)
            {
                request.tracksToExtendTo.push_back(params.at(i));
            }
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " track_id_list" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.ExtendSelectionToTargetTracks(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
