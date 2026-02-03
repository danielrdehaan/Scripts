// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::CopySpecial command.
 */

#include "Common.h"

const std::string g_pszCopySpecial = "CopySpecial";
const std::string g_pszCopySpecialHelp =
    "CopySpecial [-automation_data_option {All_Automation|Pan_Automation|PlugIn_Automation|Clip_Gain|Clip_Effects}]";

PtslCmdCommandResult CopySpecial(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CopySpecialRequest request;
    request.commandType = CommandType::CopySpecial;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-automation_data_option") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "All_Automation") == 0)
                    {
                        request.automationDataOption = AutomationDataOptions::All_Automation;
                    }
                    else if (strcmp(params.at(i).c_str(), "Pan_Automation") == 0)
                    {
                        request.automationDataOption = AutomationDataOptions::Pan_Automation;
                    }
                    else if (strcmp(params.at(i).c_str(), "PlugIn_Automation") == 0)
                    {
                        request.automationDataOption = AutomationDataOptions::PlugIn_Automation;
                    }
                    else if (strcmp(params.at(i).c_str(), "Clip_Gain") == 0)
                    {
                        request.automationDataOption = AutomationDataOptions::Clip_Gain;
                    }
                    else if (strcmp(params.at(i).c_str(), "Clip_Effects") == 0)
                    {
                        request.automationDataOption = AutomationDataOptions::Clip_Effects;
                    }
                }
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.CopySpecial(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
