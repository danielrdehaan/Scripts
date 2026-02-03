// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetTrackList command.
 */

#include "Common.h"

const std::string g_pszGetTrackList = "GetTrackList";
const std::string g_pszGetTrackListHelp =
    "GetTrackList [-limit <limit>] [-offset <offset>] [-[not_]additive] [-track_filters [not_]{all|selected|selected_explicitly|selected_implicitly|with_clips_on_main_playlist|with_automation_on_main_playlist|inactive|inactive_explicitly|inactive_implicitly|hidden|hidden_explicitly|hidden_implicitly|locked|muted|frozen|open|online} [[not_]<filter> [...]]]";

PtslCmdCommandResult GetTrackList(const std::vector<std::string>& params, CppPTSLClient& client)
{
    GetTrackListRequest request;
    request.commandType = CommandType::GetTrackList;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const std::string pageLimitParam = "limit";
        const std::string offsetParam = "offset";
        const std::string trackFiltersParam = "track_filters";
        const std::string additiveParam = "additive";
        const std::string notModifier = "not_";

        request.isFilterListAdditive = true;
        request.trackFilters = { { TrackListFilter::All, false } };

        // Separate each command line parameter's arguments:
        auto paramsWithArgs = CommandLineParser::RetrieveParamsWithArgs(params);

        // Make a request accordingly to command line params:
        for (auto pair : paramsWithArgs)
        {
            if (pair.first == CommandLineParser::PARAMETERLESS_ARGS_KEY && !pair.second.empty())
            {
                cout << CommandLineParser::PARAMETERLESS_ARGS << " " << pair.second.at(0) << endl;
                return false;
            }

            if (pair.first == pageLimitParam)
            {
                if (pair.second.size() != 1)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << pageLimitParam << endl;
                    return false;
                }

                request.paginationRequest.limit = stoi(pair.second.at(0));
            }
            else if (pair.first == offsetParam)
            {
                if (pair.second.size() != 1)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << offsetParam << endl;
                    return false;
                }

                request.paginationRequest.offset = stoi(pair.second[0]);
            }
            else if (pair.first.size() >= additiveParam.size()
                && 0
                    == pair.first.compare(pair.first.size() - additiveParam.size(),
                        additiveParam.size(),
                        additiveParam)) // if ends with "additive"
            {
                if (!pair.second.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << additiveParam << endl;
                    return false;
                }

                request.isFilterListAdditive = pair.first.at(0) != notModifier.at(0);
            }
            else if (pair.first == trackFiltersParam)
            {
                if (pair.second.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackFiltersParam << endl;
                    return false;
                }

                request.trackFilters = {};
                static const std::map<std::string, TrackListFilter> filtersMap = { { "all", TrackListFilter::All },
                    { "selected", TrackListFilter::Selected },
                    { "selected_explicitly", TrackListFilter::SelectedExplicitly },
                    { "selected_implicitly", TrackListFilter::SelectedImplicitly },
                    { "with_clips_on_main_playlist", TrackListFilter::WithClipsOnMainPlaylist },
                    { "with_automation_on_main_playlist", TrackListFilter::WithAutomationOnMainPlaylist },
                    { "inactive", TrackListFilter::Inactive },
                    { "inactive_explicitly", TrackListFilter::InactiveExplicitly },
                    { "inactive_implicitly", TrackListFilter::InactiveImplicitly },
                    { "hidden", TrackListFilter::Hidden },
                    { "hidden_explicitly", TrackListFilter::HiddenExplicitly },
                    { "hidden_implicitly", TrackListFilter::HiddenImplicitly },
                    { "locked", TrackListFilter::Locked },
                    { "muted", TrackListFilter::Muted },
                    { "frozen", TrackListFilter::Frozen },
                    { "open", TrackListFilter::Open },
                    { "online", TrackListFilter::Online } };

                for (const auto& arg : pair.second)
                {
                    std::string pureArg = arg;
                    bool isInverted = false;

                    const auto notPos = arg.find(notModifier);
                    if (notPos != std::string::npos)
                    {
                        isInverted = true;
                        pureArg.replace(notPos, notModifier.length(), "");
                    }

                    if (filtersMap.count(pureArg) == 0)
                    {
                        cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << trackFiltersParam
                             << ". There is no such a filter argument: " << pureArg << endl;
                        return false;
                    }

                    request.trackFilters.push_back({ filtersMap.at(pureArg), isInverted });
                }
            }
            else
            {
                cout << "Wrong parameter: " << pair.first << endl;
                return false;
            }
        } // (end for)
    }

    // Call the client's method with the created request:
    std::shared_ptr<GetTrackListResponse> rsp = client.GetTrackList(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        static const std::map<TrackAttributeState, std::string> trackAttrStates = { { TrackAttributeState::None,
                                                                                        "None" },
            { TrackAttributeState::SetExplicitly, "SetExplicitly" },
            { TrackAttributeState::SetImplicitly, "SetImplicitly" },
            { TrackAttributeState::SetExplicitlyAndImplicitly, "SetExplicitlyAndImplicitly" } };

        cout << "GetTrackList Response:" << endl;

        if (rsp->trackList.size())
        {
            for (auto& track : rsp->trackList)
            {
                cout << "\t"
                     << "name:"
                     << "\t" << track.name << endl;
                cout << "\t"
                     << "type:"
                     << "\t" << EnumToString(track.type) << endl;
                cout << "\t"
                     << "format:"
                     << "\t" << EnumToString(track.format) << endl;
                cout << "\t"
                     << "timebase:"
                     << "\t" << EnumToString(track.timebase) << endl;
                cout << "\t"
                     << "id:"
                     << "\t" << track.id << endl;
                cout << "\t"
                     << "index:"
                     << "\t" << track.index << endl;
                cout << "\t"
                     << "color:"
                     << "\t" << track.color << endl;
                cout << "\t"
                     << "idCompressed:"
                     << "\t" << track.idCompressed << endl;
                cout << "\t"
                     << "track_attributes:" << endl;
                cout << "\t\t"
                     << "isInactive:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isInactive) << endl;
                cout << "\t\t"
                     << "isHidden:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isHidden) << endl;
                cout << "\t\t"
                     << "isSelected:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isSelected) << endl;
                cout << "\t\t"
                     << "containsClips:"
                     << "\t" << track.trackAttributes.containsClips << endl;
                cout << "\t\t"
                     << "containsAutomation:"
                     << "\t" << track.trackAttributes.containsAutomation << endl;
                cout << "\t\t"
                     << "isSoloed:"
                     << "\t" << track.trackAttributes.isSoloed << endl;
                cout << "\t\t"
                     << "isRecordEnabled:"
                     << "\t" << track.trackAttributes.isRecordEnabled << endl;
                cout << "\t\t"
                     << "isInputMonitoringOn:"
                     << "\t" << trackAttrStates.at(track.trackAttributes.isInputMonitoringOn) << endl;
                cout << "\t\t"
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
                cout << "\t\t"
                     << "isLocked:"
                     << "\t" << track.trackAttributes.isLocked << endl;
                cout << "\t\t"
                     << "isMuted:"
                     << "\t" << track.trackAttributes.isMuted << endl;
                cout << "\t\t"
                     << "isFrozen:"
                     << "\t" << track.trackAttributes.isFrozen << endl;
                cout << "\t\t"
                     << "isOpen:"
                     << "\t" << track.trackAttributes.isOpen << endl;
                cout << "\t\t"
                     << "isOnline:"
                     << "\t" << track.trackAttributes.isOnline << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "  --No Tracks Returned--" << endl;
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "Track List Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "Track List Request return an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
