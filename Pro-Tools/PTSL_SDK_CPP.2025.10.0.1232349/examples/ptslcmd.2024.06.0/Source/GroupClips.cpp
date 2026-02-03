// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GroupClips command.
 */

#include "Common.h"

const string g_pszGroupClips = "GroupClips";
const string g_pszGroupClipsHelp = g_pszGroupClips;

PtslCmdCommandResult GroupClips(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GroupClips;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.GroupClips(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
