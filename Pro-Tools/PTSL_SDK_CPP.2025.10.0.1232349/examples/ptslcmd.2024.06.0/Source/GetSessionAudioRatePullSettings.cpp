// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionAudioRatePullSettings command.
 */

#include "Common.h"

const std::string g_pszGetSessionAudioRatePullSettings = "GetSessionAudioRatePullSettings";
const std::string g_pszGetSessionAudioRatePullSettingsHelp = g_pszGetSessionAudioRatePullSettings;

PtslCmdCommandResult GetSessionAudioRatePullSettings(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionAudioRatePullSettings;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionAudioRatePullSettingsResponse> rsp = client.GetSessionAudioRatePullSettings(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<SessionRatePull, string> enumMap = {
        MAP_ENTRY(SessionRatePull, SRP_None),
        MAP_ENTRY(SessionRatePull, SRP_Up01),
        MAP_ENTRY(SessionRatePull, SRP_Down01),
        MAP_ENTRY(SessionRatePull, SRP_Up4),
        MAP_ENTRY(SessionRatePull, SRP_Up4Up01),
        MAP_ENTRY(SessionRatePull, SRP_Up4Down01),
        MAP_ENTRY(SessionRatePull, SRP_Down4),
        MAP_ENTRY(SessionRatePull, SRP_Down4Up01),
        MAP_ENTRY(SessionRatePull, SRP_Down4Down01),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionAudioRatePullSettings Response:" << endl;
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
        cout << "GetSessionAudioRatePullSettings Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionAudioRatePullSettings returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
