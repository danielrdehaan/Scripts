// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetEditTool command.
 */

#include "Common.h"

const string g_pszSetEditTool = "SetEditTool";
const string g_pszSetEditToolHelp =
    "SetEditTool -edit_tool [ET_ZoomNormal|ET_ZoomSingle|ET_TrimStandard|ET_TrimTce|ET_TrimScrub|ET_TrimLoop|ET_Selector|ET_GrabberTime|ET_GrabberSeparation|ET_GrabberObject|ET_SmartTool|ET_Scrubber|ET_PencilFreeHand|ET_PencilLine|ET_PencilTriangle|ET_PencilSquare|ET_PencilRandom|ET_PencilParabolic|ET_PencilSCurve]";

PtslCmdCommandResult SetEditTool(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetEditToolRequest request;
    request.commandType = CommandType::SetEditTool;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string editToolParam = "edit_tool";
        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        // Populate the request by the parameters and their args provided:
        for (const auto& pair : paramsArgsMap)
        {
            const string param = pair.first;
            const vector<string> args = pair.second;

            if (param == CommandLineParser::PARAMETERLESS_ARGS_KEY && !args.empty())
            {
                cout << CommandLineParser::PARAMETERLESS_ARGS << " " << args.at(0) << endl;
                return false;
            }

            if (param == editToolParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << editToolParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, EditTool> editToolMap = {
                    // ET_Unknown isn't here because it's tech-only
                    RVRS_MAP_ENTRY(EditTool, ET_ZoomNormal),
                    RVRS_MAP_ENTRY(EditTool, ET_ZoomSingle),
                    RVRS_MAP_ENTRY(EditTool, ET_TrimStandard),
                    RVRS_MAP_ENTRY(EditTool, ET_TrimTce),
                    RVRS_MAP_ENTRY(EditTool, ET_TrimScrub),
                    RVRS_MAP_ENTRY(EditTool, ET_TrimLoop),
                    RVRS_MAP_ENTRY(EditTool, ET_Selector),
                    RVRS_MAP_ENTRY(EditTool, ET_GrabberTime),
                    RVRS_MAP_ENTRY(EditTool, ET_GrabberSeparation),
                    RVRS_MAP_ENTRY(EditTool, ET_GrabberObject),
                    RVRS_MAP_ENTRY(EditTool, ET_SmartTool),
                    RVRS_MAP_ENTRY(EditTool, ET_Scrubber),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilFreeHand),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilLine),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilTriangle),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilSquare),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilRandom),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilParabolic),
                    RVRS_MAP_ENTRY(EditTool, ET_PencilSCurve),
                };

                if (editToolMap.count(theArg) == 0)
                {
                    string errorMessage = (theArg == "ET_Unknown") ? "Don't use this enum value, it's tech-only: "
                                                                   : "There is no such an argument: ";

                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << editToolParam << ". "
                         << errorMessage << theArg << endl;
                    return false;
                }

                request.editTool = editToolMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetEditTool(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
