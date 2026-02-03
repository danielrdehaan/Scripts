// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetEditMode command.
 */

#include "Common.h"

const string g_pszGetEditMode = "GetEditMode";
const string g_pszGetEditModeHelp = g_pszGetEditMode;

PtslCmdCommandResult GetEditMode(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetEditMode;

    // Call the client's method with the created request:
    std::shared_ptr<GetEditModeResponse> rsp = client.GetEditMode(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<EditMode, string> enumMap = {
        MAP_ENTRY(EditMode, EMO_Shuffle),
        MAP_ENTRY(EditMode, EMO_Slip),
        MAP_ENTRY(EditMode, EMO_Spot),
        MAP_ENTRY(EditMode, EMO_GridAbsolute),
        MAP_ENTRY(EditMode, EMO_GridRelative),
        MAP_ENTRY(EditMode, EMO_ShuffleSnapToGridAbsolute),
        MAP_ENTRY(EditMode, EMO_SlipSnapToGridAbsolute),
        MAP_ENTRY(EditMode, EMO_SpotSnapToGridAbsolute),
        MAP_ENTRY(EditMode, EMO_ShuffleSnapToGridRelative),
        MAP_ENTRY(EditMode, EMO_SlipSnapToGridRelative),
        MAP_ENTRY(EditMode, EMO_SpotSnapToGridRelative),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetEditMode Response:" << endl;
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
        cout << "GetEditMode Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetEditMode returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
