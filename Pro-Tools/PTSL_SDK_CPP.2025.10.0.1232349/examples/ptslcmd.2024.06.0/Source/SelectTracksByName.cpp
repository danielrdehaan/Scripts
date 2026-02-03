// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SelectTracksByName command.
 */

#include "Common.h"

const std::string g_pszSelectTracksByName = "SelectTracksByName";
const std::string g_pszSelectTracksByNameHelp =
    "SelectTracksByName -track_names <track_names> [-selection_mode {Replace|Add|Subtract}] [-limit <limit>] [-offset <offset>]";

PtslCmdCommandResult SelectTracksByName(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SelectTracksByNameRequest request;
    request.commandType = CommandType::SelectTracksByName;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissed = true;
        size_t count = params.size();

        request.selectionMode = SelectionMode::SM_Replace;

        for (size_t i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-track_names") == 0)
            {
                auto paramsWithArgs = CommandLineParser::RetrieveParamsWithArgs(params);
                auto paramIter = paramsWithArgs.find("track_names");

                if (paramIter != paramsWithArgs.end())
                {
                    for (const auto& name : paramIter->second)
                    {
                        if (++i < count)
                        {
                            request.trackNames.push_back(name);
                        }
                    }
                }

                bMissed = false;
            }
            else if (strcmp(params.at(i).c_str(), "-selection_mode") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Replace") == 0)
                    {
                        request.selectionMode = SelectionMode::SM_Replace;
                    }
                    else if (strcmp(params.at(i).c_str(), "Add") == 0)
                    {
                        request.selectionMode = SelectionMode::SM_Add;
                    }
                    else if (strcmp(params.at(i).c_str(), "Subtract") == 0)
                    {
                        request.selectionMode = SelectionMode::SM_Subtract;
                    }
                }
            }
            else if (strcmp(params.at(i).c_str(), "-limit") == 0)
            {
                if (++i < count)
                {
                    request.paginationRequest.limit = stoi(params.at(i));
                }
            }
            else if (strcmp(params.at(i).c_str(), "-offset") == 0)
            {
                if (++i < count)
                {
                    request.paginationRequest.offset = stoi(params.at(i));
                }
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " track_names" << endl;
            return false;
        }
    }

    std::shared_ptr<SelectTracksByNameResponse> rsp = client.SelectTracksByName(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        static const std::map<TrackType, std::string> trackTypes = { { TrackType::TT_Unknown, "Unknown" },
            { TrackType::TT_Midi, "Midi" },
            { TrackType::TT_Audio, "Audio" },
            { TrackType::TT_Aux, "Aux" },
            { TrackType::TT_Video, "Video" },
            { TrackType::TT_Vca, "Vca" },
            { TrackType::TT_Tempo, "Tempo" },
            { TrackType::TT_Markers, "Markers" },
            { TrackType::TT_Meter, "Meter" },
            { TrackType::TT_KeySignature, "KeySignature" },
            { TrackType::TT_ChordSymbols, "ChordSymbols" },
            { TrackType::TT_Instrument, "Instrument" },
            { TrackType::TT_Master, "Master" },
            { TrackType::TT_Heat, "Heat" },
            { TrackType::TT_BasicFolder, "BasicFolder" },
            { TrackType::TT_RoutingFolder, "RoutingFolder" },
            { TrackType::TT_CompLane, "CompLane" } };
        static const std::map<TrackAttributeState, std::string> trackAttrStates = { { TrackAttributeState::None,
                                                                                        "None" },
            { TrackAttributeState::SetExplicitly, "SetExplicitly" },
            { TrackAttributeState::SetImplicitly, "SetImplicitly" },
            { TrackAttributeState::SetExplicitlyAndImplicitly, "SetExplicitlyAndImplicitly" } };

        cout << "SelectTracksByName Response:" << endl;

        if (rsp->selectedTracks.size())
        {
            static const std::map<SelectionMode, std::string> selectionModeWords { { SelectionMode::SM_Add,
                                                                                       "extended" },
                { SelectionMode::SM_Replace, "changed" },
                { SelectionMode::SM_Subtract, "reduced" } };

            cout << "\tThe track selection was " << selectionModeWords.at(request.selectionMode)
                 << ". The current selection is:" << endl;

            for (auto& track : rsp->selectedTracks)
            {
                cout << "\t\t"
                     << "name:"
                     << "\t" << track.name << endl;
                cout << "\t\t"
                     << "type:"
                     << "\t" << trackTypes.at(track.type) << endl;
                cout << "\t"
                     << "format:"
                     << "\t" << EnumToString(track.format) << endl;
                cout << "\t"
                     << "timebase:"
                     << "\t" << EnumToString(track.timebase) << endl;
                cout << "\t\t"
                     << "id:"
                     << "\t" << track.id << endl;
                cout << "\t\t"
                     << "index:"
                     << "\t" << track.index << endl;
                cout << "\t\t"
                     << "color:"
                     << "\t" << track.color << endl;
                cout << "\t\t"
                     << "idCompressed:"
                     << "\t" << track.idCompressed << endl;
                cout << "\t\t"
                     << "track_attributes:" << endl;
                cout << "\t\t\t"
                     << "isInactive:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isInactive) << endl;
                cout << "\t\t\t"
                     << "isHidden:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isHidden) << endl;
                cout << "\t\t\t"
                     << "isSelected:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isSelected) << endl;
                cout << "\t\t\t"
                     << "containsClips:"
                     << "\t" << track.trackAttributes.containsClips << endl;
                cout << "\t\t\t"
                     << "containsAutomation:"
                     << "\t" << track.trackAttributes.containsAutomation << endl;
                cout << "\t\t\t"
                     << "isSoloed:"
                     << "\t" << track.trackAttributes.isSoloed << endl;
                cout << "\t\t\t"
                     << "isRecordEnabled:"
                     << "\t" << track.trackAttributes.isRecordEnabled << endl;
                cout << "\t\t\t"
                     << "isInputMonitoringOn:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isInputMonitoringOn) << endl;
                cout << "\t\t\t"
                     << "isSmartDspOn:"
                     << "\t" << track.trackAttributes.isSmartDspOn << endl;
                cout << "\t\t"
                     << "isRecordEnabledSafe:"
                     << "\t" << track.trackAttributes.isRecordEnabledSafe << endl;
                cout << "\t\t"
                     << "isSmartDspOnSafe:"
                     << "\t" << track.trackAttributes.isSmartDspOnSafe << endl;
                cout << "\t\t"
                     << "isSoloedSafe:"
                     << "\t" << track.trackAttributes.isSoloedSafe << endl;
                cout << "\t\t\t"
                     << "isLocked:"
                     << "\t" << track.trackAttributes.isLocked << endl;
                cout << "\t\t\t"
                     << "isMuted:"
                     << "\t" << track.trackAttributes.isMuted << endl;
                cout << "\t\t\t"
                     << "isFrozen:"
                     << "\t" << track.trackAttributes.isFrozen << endl;
                cout << "\t\t\t"
                     << "isOpen:"
                     << "\t" << track.trackAttributes.isOpen << endl;
                cout << "\t\t\t"
                     << "isOnline:"
                     << "\t" << track.trackAttributes.isOnline << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "  --No Selected Tracks Returned--" << endl;
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "SelectTracksByName Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "SelectTracksByName Request return an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
