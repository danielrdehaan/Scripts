// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetEditTool command.
 */

#include "Common.h"

const string g_pszGetEditTool = "GetEditTool";
const string g_pszGetEditToolHelp = g_pszGetEditTool;

PtslCmdCommandResult GetEditTool(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetEditTool;

    // Call the client's method with the created request:
    std::shared_ptr<GetEditToolResponse> rsp = client.GetEditTool(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<EditTool, string> enumMap = {
        MAP_ENTRY(EditTool, ET_ZoomNormal),
        MAP_ENTRY(EditTool, ET_ZoomSingle),
        MAP_ENTRY(EditTool, ET_TrimStandard),
        MAP_ENTRY(EditTool, ET_TrimTce),
        MAP_ENTRY(EditTool, ET_TrimScrub),
        MAP_ENTRY(EditTool, ET_TrimLoop),
        MAP_ENTRY(EditTool, ET_Selector),
        MAP_ENTRY(EditTool, ET_GrabberTime),
        MAP_ENTRY(EditTool, ET_GrabberSeparation),
        MAP_ENTRY(EditTool, ET_GrabberObject),
        MAP_ENTRY(EditTool, ET_SmartTool),
        MAP_ENTRY(EditTool, ET_Scrubber),
        MAP_ENTRY(EditTool, ET_PencilFreeHand),
        MAP_ENTRY(EditTool, ET_PencilLine),
        MAP_ENTRY(EditTool, ET_PencilTriangle),
        MAP_ENTRY(EditTool, ET_PencilSquare),
        MAP_ENTRY(EditTool, ET_PencilRandom),
        MAP_ENTRY(EditTool, ET_PencilParabolic),
        MAP_ENTRY(EditTool, ET_PencilSCurve),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetEditTool Response:" << endl;
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
        cout << "GetEditTool Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetEditTool returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
