// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SelectAllClipsOnTrack command.
 */

#include "Common.h"

const std::string g_pszSelectAllClipsOnTrack = "SelectAllClipsOnTrack";
const std::string g_pszSelectAllClipsOnTrackHelp = "SelectAllClipsOnTrack -track_name <track_name>";

PtslCmdCommandResult SelectAllClipsOnTrack(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SelectAllClipsOnTrackRequest request;
    request.commandType = CommandType::SelectAllClipsOnTrack;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        if (params.size() > 1 && strcmp(params.at(0).c_str(), "-track_name") == 0)
        {
            request.trackName = params.at(1);
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " track_name" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SelectAllClipsOnTrack(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
