// Copyright 2020, 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RenameTrack command.
 */

#include "Common.h"

const std::string g_pszRenameTargetTrack = "RenameTargetTrack";
const std::string g_pszRenameTargetTrackHelp = "RenameTargetTrack -current_name <current_name> -new_name <new_name>";

PtslCmdCommandResult RenameTargetTrack(const std::vector<std::string>& params, CppPTSLClient& client)
{
    RenameTargetTrackRequest request;
    request.commandType = CommandType::RenameTargetTrack;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissedCurrentName = true;
        bool bMissedNewName = true;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-track_id") == 0)
            {
                if (++i < count)
                {
                    request.trackId = params.at(i);
                    bMissedCurrentName = false;
                }
            }
            if (strcmp(params.at(i).c_str(), "-current_name") == 0)
            {
                if (++i < count)
                {
                    request.currentTrackName = params.at(i);
                    bMissedCurrentName = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-new_name") == 0)
            {
                if (++i < count)
                {
                    request.newTrackName = params.at(i);
                    bMissedNewName = false;
                }
            }
        }

        if (bMissedCurrentName || bMissedNewName)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (bMissedCurrentName)
            {
                cout << " current_name";
            }

            if (bMissedNewName)
            {
                cout << " new_name";
            }

            cout << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.RenameTargetTrack(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
