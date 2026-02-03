// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RenameSelectedClip command.
 */

#include "Common.h"

const std::string g_pszRenameSelectedClip = "RenameSelectedClip";
const std::string g_pszRenameSelectedClipHelp =
    "RenameSelectedClip -clip_location {ClipsList|Timeline} -new_name <new_name> [-rename_file]";

PtslCmdCommandResult RenameSelectedClip(const std::vector<std::string>& params, CppPTSLClient& client)
{
    RenameSelectedClipRequest request;
    request.commandType = CommandType::RenameTargetClip;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissedClipLocation = true;
        bool bMissedNewName = true;

        request.renameFile = false;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-clip_location") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "ClipsList") == 0)
                    {
                        request.clipLocation = CL_ClipLocation::CL_ClipsList;
                    }
                    else if (strcmp(params.at(i).c_str(), "Timeline") == 0)
                    {
                        request.clipLocation = CL_ClipLocation::CL_Timeline;
                    }

                    bMissedClipLocation = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-new_name") == 0)
            {
                if (++i < count)
                {
                    request.newName = params.at(i);
                    bMissedNewName = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-rename_file") == 0)
            {
                request.renameFile = true;
                request.renameFileExplicitly = true;
            }
        }

        if (bMissedClipLocation || bMissedNewName)
        {
            cout << CommandLineParser::PARAMETER_MISSED;

            if (bMissedClipLocation)
            {
                cout << " clip_location";
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
    std::shared_ptr<CommandResponse> rsp = client.RenameSelectedClip(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
