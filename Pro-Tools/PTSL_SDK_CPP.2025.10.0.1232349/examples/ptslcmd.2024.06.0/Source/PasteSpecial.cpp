// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::PasteSpecial command.
 */

#include "Common.h"

const std::string g_pszPasteSpecial = "PasteSpecial";
const std::string g_pszPasteSpecialHelp =
    "PasteSpecial [-paste_special_option {MergeMidi|MergeMarkers|Repeat_To_Fill_Selection|To_Current_Automation_Type}]";

PtslCmdCommandResult PasteSpecial(const std::vector<std::string>& params, CppPTSLClient& client)
{
    PasteSpecialRequest request;
    request.commandType = CommandType::PasteSpecial;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-paste_special_option") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Merge") == 0 || strcmp(params.at(i).c_str(), "MergeMidi") == 0)
                    {
                        request.pasteSpecialOption = PasteSpecialOptions::MergeMidi;
                    }
                    else if (strcmp(params.at(i).c_str(), "MergeMarkers") == 0)
                    {
                        request.pasteSpecialOption = PasteSpecialOptions::MergeMarkers;
                    }
                    else if (strcmp(params.at(i).c_str(), "Repeat_To_Fill_Selection") == 0)
                    {
                        request.pasteSpecialOption = PasteSpecialOptions::Repeat_To_Fill_Selection;
                    }
                    else if (strcmp(params.at(i).c_str(), "To_Current_Automation_Type") == 0)
                    {
                        request.pasteSpecialOption = PasteSpecialOptions::To_Current_Automation_Type;
                    }
                }
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.PasteSpecial(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
