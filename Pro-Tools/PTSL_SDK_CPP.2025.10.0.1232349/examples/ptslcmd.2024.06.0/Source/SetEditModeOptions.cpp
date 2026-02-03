// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetEditModeOptions command.
 */

#include "Common.h"

const string g_pszSetEditModeOptions = "SetEditModeOptions";
const string g_pszSetEditModeOptionsHelp =
    "SetEditModeOptions [-tab_to_transients <tabToTransients>] [-link_timeline_and_edit_selection <linkTimelineAndEditSelection>] [-link_track_and_edit_selection <linkTrackAndEditSelection>] [-insertion_follows_playback <insertionFollowsPlayback>] [-automation_follows_edit <automationFollowsEdit>] [-markers_follow_edit <markersFollowEdit>] [-mirrored_midi_editing <mirroredMidiEditing>] [-layered_editing <layeredEditing>]";

PtslCmdCommandResult SetEditModeOptions(const vector<string>& params, CppPTSLClient& client)
{
    SetEditModeOptionsRequest request;
    request.commandType = CommandType::SetEditModeOptions;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.editModeOptions.tabToTransients = false;
        request.editModeOptions.linkTimelineAndEditSelection = false;
        request.editModeOptions.linkTrackAndEditSelection = false;
        request.editModeOptions.insertionFollowsPlayback = false;
        request.editModeOptions.automationFollowsEdit = false;
        request.editModeOptions.markersFollowEdit = false;
        request.editModeOptions.mirroredMidiEditing = false;
        request.editModeOptions.layeredEditing = false;

        const string tabToTransientsParam = "tab_to_transients";
        const string linkTimelineAndEditSelectionParam = "link_timeline_and_edit_selection";
        const string linkTrackAndEditSelectionParam = "link_track_and_edit_selection";
        const string insertionFollowsPlaybackParam = "insertion_follows_playback";
        const string automationFollowsEditParam = "automation_follows_edit";
        const string markersFollowEditParam = "markers_follow_edit";
        const string mirroredMidiEditingParam = "mirrored_midi_editing";
        const string layeredEditingParam = "layered_editing";

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

            if (param == tabToTransientsParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << tabToTransientsParam << endl;
                    return false;
                }

                request.editModeOptions.tabToTransients = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == linkTimelineAndEditSelectionParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << linkTimelineAndEditSelectionParam
                         << endl;
                    return false;
                }

                request.editModeOptions.linkTimelineAndEditSelection = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == linkTrackAndEditSelectionParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << linkTrackAndEditSelectionParam
                         << endl;
                    return false;
                }

                request.editModeOptions.linkTrackAndEditSelection = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == insertionFollowsPlaybackParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << insertionFollowsPlaybackParam
                         << endl;
                    return false;
                }

                request.editModeOptions.insertionFollowsPlayback = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == automationFollowsEditParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << automationFollowsEditParam << endl;
                    return false;
                }

                request.editModeOptions.automationFollowsEdit = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == markersFollowEditParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << markersFollowEditParam << endl;
                    return false;
                }

                request.editModeOptions.markersFollowEdit = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == mirroredMidiEditingParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << mirroredMidiEditingParam << endl;
                    return false;
                }

                request.editModeOptions.mirroredMidiEditing = CommandLineParser::StringToBool(args.at(0));
            }
            else if (param == layeredEditingParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << layeredEditingParam << endl;
                    return false;
                }

                request.editModeOptions.layeredEditing = CommandLineParser::StringToBool(args.at(0));
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetEditModeOptions(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
