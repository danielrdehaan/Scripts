// Copyright 2020, 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetTaskStatus command.
 */

#include "Common.h"

const std::string g_pszGetTaskStatus = "GetTaskStatus";
const std::string g_pszGetTaskStatusHelp = "GetTaskStatus -task_id <task_id>";

PtslCmdCommandResult GetTaskStatus(const std::vector<std::string>& params, CppPTSLClient& client)
{
    GetTaskStatusRequest request;
    request.commandType = CommandType::GetTaskStatus;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        if (params.size() > 1 && strcmp(params.at(0).c_str(), "-task_id") == 0)
        {
            request.requestedTaskId = params.at(1);
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " task_id" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.GetTaskStatus(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
