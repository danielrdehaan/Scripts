// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RepeatSelection command.
 */

#include "Common.h"

const std::string g_pszRepeatSelection = "RepeatSelection";
const std::string g_pszRepeatSelectionHelp = "RepeatSelection [-num_repeats <num_repeats>]";

PtslCmdCommandResult RepeatSelection(const std::vector<std::string>& params, CppPTSLClient& client)
{
    RepeatSelectionRequest request;
    request.commandType = CommandType::RepeatSelection;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.numRepeats = 1;

        if (params.size() > 1 && strcmp(params.at(0).c_str(), "-num_repeats") == 0)
        {
            request.numRepeats = stoi(params.at(1).c_str());
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.RepeatSelection(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
