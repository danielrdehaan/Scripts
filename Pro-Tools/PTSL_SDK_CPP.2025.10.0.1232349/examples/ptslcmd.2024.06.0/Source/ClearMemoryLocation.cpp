// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ClearMemoryLocation command.
 */

#include "Common.h"

const std::string g_pszClearMemoryLocation = "ClearMemoryLocation";
const std::string g_pszClearMemoryLocationHelp =
    "ClearMemoryLocation -location_list <location_list> (0 or less to clear all memory locations)";

PtslCmdCommandResult ClearMemoryLocation(const std::vector<std::string>& params, CppPTSLClient& client)
{
    ClearMemoryLocationRequest request;
    request.commandType = CommandType::ClearMemoryLocation;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        size_t count = params.size();

        if (count > 1 && strcmp(params.at(0).c_str(), "-location_list") == 0)
        {
            for (int i = 1; i < count; ++i)
            {
                request.locationList.push_back(static_cast<int64_t>(atoi(params.at(i).c_str())));
            }
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " location_list" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<ClearMemoryLocationResponse> rsp = client.ClearMemoryLocation(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "ClearMemoryLocation Response:" << endl;

        cout << "\t"
             << "Count of successfully cleared memory locations: " << rsp->successCount << endl;
        cout << "\t"
             << "Count of failures during clearing memory locations: " << rsp->failureCount << endl;

        if (rsp->failureCount > 0)
        {
            cout << "\t"
                 << "Failures list: " << endl;
            for (const auto& failure : rsp->failureList)
            {
                cout << "\t\t" << failure << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "ClearMemoryLocation Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << "\t" << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "ClearMemoryLocation return an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
