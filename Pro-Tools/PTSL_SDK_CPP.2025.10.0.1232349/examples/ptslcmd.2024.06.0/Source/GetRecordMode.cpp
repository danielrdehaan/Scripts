// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetRecordMode command.
 */

#include "Common.h"

const std::string g_pszGetRecordMode = "GetRecordMode";
const std::string g_pszGetRecordModeHelp = g_pszGetRecordMode;

PtslCmdCommandResult GetRecordMode(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetRecordMode;

    // Call the client's method with the created request:
    std::shared_ptr<GetRecordModeResponse> rsp = client.GetRecordMode(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<RM_RecordMode, string> enumMap = {
        MAP_ENTRY(RM_RecordMode, RM_Normal),
        MAP_ENTRY(RM_RecordMode, RM_Loop),
        MAP_ENTRY(RM_RecordMode, RM_Destructive),
        MAP_ENTRY(RM_RecordMode, RM_QuickPunch),
        MAP_ENTRY(RM_RecordMode, RM_TrackPunch),
        MAP_ENTRY(RM_RecordMode, RM_DestructivePunch),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetRecordMode Response:" << endl;
        cout << "\t"
             << "current setting:"
             << "\t" << (enumMap.count(rsp->currentSetting) > 0 ? enumMap.at(rsp->currentSetting) : "") << endl;

        if (rsp->possibleSettings.size())
        {
            cout << "\t"
                 << "possible settings:" << endl;

            for (auto& item : rsp->possibleSettings)
            {
                cout << "\t\t" << (enumMap.count(item) > 0 ? enumMap.at(item) : "") << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetRecordMode Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetRecordMode returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
