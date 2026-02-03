// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::CreateFade command.
 */

#include "Common.h"

const std::string g_pszCreateFadesBasedOnPreset = "CreateFadesBasedOnPreset";
const std::string g_pszCreateFadesBasedOnPresetHelp = "CreateFadesBasedOnPreset -preset <preset> [-auto_adjust_bounds]";

PtslCmdCommandResult CreateFadesBasedOnPreset(const std::vector<std::string>& params, CppPTSLClient& client)
{
    bool bMissed = true;

    CreateFadesBasedOnPresetRequest request;
    request.commandType = CommandType::CreateFadesBasedOnPreset;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.autoAdjustBounds = true;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-preset") == 0)
            {
                if (++i < count)
                {
                    request.fadePresetName = params.at(i);
                    bMissed = false;
                }
            }
            else if (strcmp(params.at(i).c_str(), "-auto_adjust_bounds") == 0)
            {
                if (++i < count)
                {
                    bool tmpAutoAdjustBounds = false;
                    std::istringstream(params.at(i)) >> std::boolalpha >> tmpAutoAdjustBounds;

                    request.autoAdjustBounds = tmpAutoAdjustBounds;
                }
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " preset" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.CreateFadesBasedOnPreset(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
