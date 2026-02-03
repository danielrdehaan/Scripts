// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ClearUndoQueue command.
 */

#include "Common.h"

const string g_pszClearUndoQueue = "ClearUndoQueue";
const string g_pszClearUndoQueueHelp = g_pszClearUndoQueue;

PtslCmdCommandResult ClearUndoQueue(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::ClearUndoQueue;

    // Call the client's method with the created request:
    auto rsp = client.ClearUndoQueue(request);

    return PtslCmdCommandResult(rsp);
}
