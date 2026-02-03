// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionFeetFramesRate command.
 */

#include "Common.h"

const std::string g_pszGetSessionFeetFramesRate = "GetSessionFeetFramesRate";
const std::string g_pszGetSessionFeetFramesRateHelp = g_pszGetSessionFeetFramesRate;

PtslCmdCommandResult GetSessionFeetFramesRate(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionFeetFramesRate;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionFeetFramesRateResponse> rsp = client.GetSessionFeetFramesRate(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<SessionFeetFramesRate, string> enumMap = {
        MAP_ENTRY(SessionFeetFramesRate, SFFR_Fps23976),
        MAP_ENTRY(SessionFeetFramesRate, SFFR_Fps24),
        MAP_ENTRY(SessionFeetFramesRate, SFFR_Fps25),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionFeetFramesRate Response:" << endl;
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
        cout << "GetSessionFeetFramesRate Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionFeetFramesRate returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
