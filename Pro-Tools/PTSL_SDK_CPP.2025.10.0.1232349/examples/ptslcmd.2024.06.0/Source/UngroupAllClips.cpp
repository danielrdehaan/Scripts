// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::UngroupAllClips command.
 */

#include "Common.h"

const string g_pszUngroupAllClips = "UngroupAllClips";
const string g_pszUngroupAllClipsHelp = g_pszUngroupAllClips;

PtslCmdCommandResult UngroupAllClips(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::UngroupAllClips;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.UngroupAllClips(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
