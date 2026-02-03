// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSubCounterFormat command.
 */

#include "Common.h"

const string g_pszGetSubCounterFormat = "GetSubCounterFormat";
const string g_pszGetSubCounterFormatHelp = g_pszGetSubCounterFormat;

PtslCmdCommandResult GetSubCounterFormat(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetSubCounterFormat;

    // Call the client's method with the created request:
    std::shared_ptr<GetSubCounterFormatResponse> rsp = client.GetSubCounterFormat(request);

    // Output the response:
    if (!rsp)
    {
        cout << "No response received." << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSubCounterFormat Response:" << endl;

        cout << "\t"
             << "get sub counter format:"
             << "\t" << EnumToString(rsp->currentSetting) << endl;

        if (rsp->possibleSettings.size())
        {
            cout << "\t"
                 << "possible settings:" << endl;
            for (auto& item : rsp->possibleSettings)
            {
                cout << "		" << EnumToString(item) << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSubCounterFormat Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSubCounterFormat returned an unexpected status" << endl;
    }
    return PtslCmdCommandResult(rsp);
}
