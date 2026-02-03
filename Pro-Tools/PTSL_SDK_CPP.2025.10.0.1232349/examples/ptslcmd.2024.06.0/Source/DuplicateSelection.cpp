// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::DuplicateSelection command.
 */

#include "Common.h"

const std::string g_pszDuplicateSelection = "DuplicateSelection";
const std::string g_pszDuplicateSelectionHelp = g_pszDuplicateSelection;

PtslCmdCommandResult DuplicateSelection(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::DuplicateSelection;

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.DuplicateSelection(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
