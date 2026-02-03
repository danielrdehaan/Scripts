// Copyright 2020, 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RefreshAllModifiedAudioFiles command.
 */

#include "Common.h"

const std::string g_pszRefreshAllModifiedAudioFiles = "RefreshAllModifiedAudioFiles";
const std::string g_pszRefreshAllModifiedAudioFilesHelp = g_pszRefreshAllModifiedAudioFiles;

PtslCmdCommandResult RefreshAllModifiedAudioFiles(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::RefreshAllModifiedAudioFiles;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.RefreshAllModifiedAudioFiles(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
