// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionTimeCodeRate command.
 */

#include "Common.h"

const std::string g_pszGetSessionTimeCodeRate = "GetSessionTimeCodeRate";
const std::string g_pszGetSessionTimeCodeRateHelp = g_pszGetSessionTimeCodeRate;

PtslCmdCommandResult GetSessionTimeCodeRate(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionTimeCodeRate;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionTimeCodeRateResponse> rsp = client.GetSessionTimeCodeRate(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<SessionTimeCodeRate, string> enumMap = {
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps23976),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps24),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps25),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps2997),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps2997Drop),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps30),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps30Drop),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps47952),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps48),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps50),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps5994),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps5994Drop),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps60),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps60Drop),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps100),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps11988),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps11988Drop),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps120),
        MAP_ENTRY(SessionTimeCodeRate, STCR_Fps120Drop),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionTimeCodeRate Response:" << endl;
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
        cout << "GetSessionTimeCodeRate Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionTimeCodeRate returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
