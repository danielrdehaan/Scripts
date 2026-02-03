// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetEditMode command.
 */

#include "Common.h"

const string g_pszSetEditMode = "SetEditMode";
const string g_pszSetEditModeHelp =
    "SetEditMode -edit_mode [EMO_Shuffle|EMO_Slip|EMO_Spot|EMO_GridAbsolute|EMO_GridRelative|EMO_ShuffleSnapToGridAbsolute|EMO_SlipSnapToGridAbsolute|EMO_SpotSnapToGridAbsolute|EMO_ShuffleSnapToGridRelative|EMO_SlipSnapToGridRelative|EMO_SpotSnapToGridRelative]";

PtslCmdCommandResult SetEditMode(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetEditModeRequest request;
    request.commandType = CommandType::SetEditMode;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string editModeParam = "edit_mode";
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

            if (param == editModeParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << editModeParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, EditMode> editModeMap = {
                    // EMO_Unknown isn't here because it's tech-only
                    RVRS_MAP_ENTRY(EditMode, EMO_Shuffle),
                    RVRS_MAP_ENTRY(EditMode, EMO_Slip),
                    RVRS_MAP_ENTRY(EditMode, EMO_Spot),
                    RVRS_MAP_ENTRY(EditMode, EMO_GridAbsolute),
                    RVRS_MAP_ENTRY(EditMode, EMO_GridRelative),
                    RVRS_MAP_ENTRY(EditMode, EMO_ShuffleSnapToGridAbsolute),
                    RVRS_MAP_ENTRY(EditMode, EMO_SlipSnapToGridAbsolute),
                    RVRS_MAP_ENTRY(EditMode, EMO_SpotSnapToGridAbsolute),
                    RVRS_MAP_ENTRY(EditMode, EMO_ShuffleSnapToGridRelative),
                    RVRS_MAP_ENTRY(EditMode, EMO_SlipSnapToGridRelative),
                    RVRS_MAP_ENTRY(EditMode, EMO_SpotSnapToGridRelative),
                };

                if (editModeMap.count(theArg) == 0)
                {
                    string errorMessage = (theArg == "EMO_Unknown") ? "Don't use this enum value, it's tech-only: "
                                                                    : "There is no such an argument: ";

                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << editModeParam << ". "
                         << errorMessage << theArg << endl;
                    return false;
                }

                request.editMode = editModeMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetEditMode(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
