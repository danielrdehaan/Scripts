// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionPath command.
 */

#include "Common.h"

const std::string g_pszGetSessionPath = "GetSessionPath";
const std::string g_pszGetSessionPathHelp = g_pszGetSessionPath;

PtslCmdCommandResult GetSessionPath(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionPath;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionPathResponse> rsp = client.GetSessionPath(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionPath Response:" << endl;
        cout << "\t"
             << "session path:"
             << "\t" << rsp->sessionPath.path << endl;
        cout << "\t"
             << "session path is online:"
             << "\t" << (rsp->sessionPath.info.isOnline ? "Yes" : "No") << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionPath Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionPath returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
