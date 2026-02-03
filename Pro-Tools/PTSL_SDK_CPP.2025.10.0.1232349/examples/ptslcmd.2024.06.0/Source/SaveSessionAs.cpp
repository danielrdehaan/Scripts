// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SaveSessionAs command.
 */

#include "Common.h"

const std::string g_pszSaveSessionAs = "SaveSessionAs";
const std::string g_pszSaveSessionAsHelp =
    "SaveSessionAs -session_name <session_name> -session_location <session_location>";

PtslCmdCommandResult SaveSessionAs(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SaveSessionAsRequest request;
    request.commandType = CommandType::SaveSessionAs;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissedSessionName = true;
        bool bMissedSessionLocation = true;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-session_name") == 0)
            {
                if (++i < count)
                {
                    request.sessionName = params.at(i);
                    bMissedSessionName = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-session_location") == 0)
            {
                if (++i < count)
                {
                    request.sessionLocation = params.at(i);
                    bMissedSessionLocation = false;
                }
            }
        }

        if (bMissedSessionName || bMissedSessionLocation)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (bMissedSessionName)
            {
                cout << " session_name";
            }

            if (bMissedSessionLocation)
            {
                cout << " session_location";
            }

            cout << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SaveSessionAs(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
