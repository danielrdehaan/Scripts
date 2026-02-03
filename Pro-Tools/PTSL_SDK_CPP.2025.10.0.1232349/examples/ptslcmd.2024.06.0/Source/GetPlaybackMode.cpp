// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetPlaybackMode command.
 */

#include "Common.h"

const std::string g_pszGetPlaybackMode = "GetPlaybackMode";
const std::string g_pszGetPlaybackModeHelp = g_pszGetPlaybackMode;

PtslCmdCommandResult GetPlaybackMode(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetPlaybackMode;

    // Call the client's method with the created request:
    std::shared_ptr<GetPlaybackModeResponse> rsp = client.GetPlaybackMode(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<PM_PlaybackMode, string> enumMap = {
        MAP_ENTRY(PM_PlaybackMode, PM_Normal),
        MAP_ENTRY(PM_PlaybackMode, PM_Loop),
        MAP_ENTRY(PM_PlaybackMode, PM_DynamicTransport),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetPlaybackMode Response:" << endl;

        if (rsp->currentSettings.size())
        {
            cout << "\t"
                 << "current settings:" << endl;

            for (auto& item : rsp->currentSettings)
            {
                cout << "\t\t" << (enumMap.count(item) > 0 ? enumMap.at(item) : "") << endl;
            }
        }

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
        cout << "GetPlaybackMode Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetPlaybackMode returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
