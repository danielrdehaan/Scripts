// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetEditModeOptions command.
 */

#include "Common.h"

const string g_pszGetEditModeOptions = "GetEditModeOptions";
const string g_pszGetEditModeOptionsHelp = g_pszGetEditModeOptions;

PtslCmdCommandResult GetEditModeOptions(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    CommandRequest request;
    request.commandType = CommandType::GetEditModeOptions;

    // Call the client's method with the created request:
    std::shared_ptr<GetEditModeOptionsResponse> rsp = client.GetEditModeOptions(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetEditModeOptions Response:" << endl;
        cout << "\t"
             << "edit mode options:" << endl;

        cout << "\t\t"
             << "tabToTransients:"
             << "\t" << rsp->editModeOptions.tabToTransients << endl;
        cout << "\t\t"
             << "linkTimelineAndEditSelection:"
             << "\t" << rsp->editModeOptions.linkTimelineAndEditSelection << endl;
        cout << "\t\t"
             << "linkTrackAndEditSelection:"
             << "\t" << rsp->editModeOptions.linkTrackAndEditSelection << endl;
        cout << "\t\t"
             << "insertionFollowsPlayback:"
             << "\t" << rsp->editModeOptions.insertionFollowsPlayback << endl;
        cout << "\t\t"
             << "automationFollowsEdit:"
             << "\t" << rsp->editModeOptions.automationFollowsEdit << endl;
        cout << "\t\t"
             << "markersFollowEdit:"
             << "\t" << rsp->editModeOptions.markersFollowEdit << endl;
        cout << "\t\t"
             << "mirroredMidiEditing:"
             << "\t" << rsp->editModeOptions.mirroredMidiEditing << endl;
        cout << "\t\t"
             << "layeredEditing:"
             << "\t" << rsp->editModeOptions.layeredEditing << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetEditModeOptions Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetEditModeOptions returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
