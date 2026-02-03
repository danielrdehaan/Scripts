// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetMemoryLocations command.
 */

#include "Common.h"
const string g_pszGetMemoryLocations = "GetMemoryLocations";
const string g_pszGetMemoryLocationsHelp = "GetMemoryLocations [-limit <limit>][-offset <offset>]";

PtslCmdCommandResult GetMemoryLocations(const vector<string>& params, CppPTSLClient& client)
{
    // Create a request:
    GetMemoryLocationsRequest request;
    request.commandType = CommandType::GetMemoryLocations;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const std::string pageLimitParam = "limit";
        const std::string offsetParam = "offset";


        // Separate each command line parameter's arguments:
        auto paramsWithArgs = CommandLineParser::RetrieveParamsWithArgs(params);

        // Make a request accordingly to command line params:
        for (auto pair : paramsWithArgs)
        {
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
            else
            {
                cout << "Wrong parameter: " << pair.first << endl;
                return false;
            }
        } // (end for)
    }

    // Call the client's method with the request:
    std::shared_ptr<GetMemoryLocationsResponse> rsp = client.GetMemoryLocations(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<TimeProperties, string> timePropertiesMap = {
        MAP_ENTRY(TimeProperties, TP_Marker),
        MAP_ENTRY(TimeProperties, TP_Selection),
        MAP_ENTRY(TimeProperties, TP_None),
    };

    const std::map<MemoryLocationReference, string> memoryLocationReferenceMap = {
        MAP_ENTRY(MemoryLocationReference, MLR_BarBeat),
        MAP_ENTRY(MemoryLocationReference, MLR_Absolute),
    };

    const std::map<MarkerLocation, string> markerLocationReferenceMap = {
        MAP_ENTRY(MarkerLocation, MLC_MainRuler),
        MAP_ENTRY(MarkerLocation, MLC_Track),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetMemoryLocations Response:" << endl;

        if (rsp->memoryLocations.size())
        {
            cout << "\t"
                 << "memory locations:" << endl;
            for (auto& item : rsp->memoryLocations)
            {
                cout << "\t\t number: " << item.number << endl;
                cout << "\t\t startTime: " << item.startTime << endl;
                cout << "\t\t (endTime): " << item.endTime << endl;
                cout << "\t\t name: " << item.name << endl;
                cout << "\t\t comments: " << item.comments << endl;

                cout << "\t\t time properties: "
                     << (timePropertiesMap.count(item.timeProperties) > 0 ? timePropertiesMap.at(item.timeProperties)
                                                                          : "")
                     << endl;
                cout << "\t\t reference: "
                     << (memoryLocationReferenceMap.count(item.reference) > 0
                                ? memoryLocationReferenceMap.at(item.reference)
                                : "")
                     << endl;
                cout << "\t\t location: "
                     << (markerLocationReferenceMap.count(item.location) > 0
                                ? markerLocationReferenceMap.at(item.location)
                                : "")
                     << endl;
                cout << "\t\t track name: " << item.trackName << endl;
                cout << "\t\t color index: " << item.colorIndex << endl;

                cout << "\t\t general properties: " << endl;
                cout << "\t\t\t\t zoom settings: " << item.generalProperties.zoomSettings << endl;
                cout << "\t\t\t\t pre/post roll times: " << item.generalProperties.prePostRollTimes << endl;
                cout << "\t\t\t\t track visibility: " << item.generalProperties.trackVisibility << endl;
                cout << "\t\t\t\t track heights: " << item.generalProperties.trackHeights << endl;
                cout << "\t\t\t\t group enables: " << item.generalProperties.groupEnables << endl;
                cout << "\t\t\t\t window configuration: " << item.generalProperties.windowConfiguration << endl;
                cout << "\t\t\t\t (window configuration index): " << item.generalProperties.windowConfigurationIndex
                     << endl;
                cout << "\t\t\t\t (window configuration name): " << item.generalProperties.windowConfigurationName
                     << endl;
                cout << "\t\t\t\t venue index: " << item.generalProperties.venueSnapshotIndex << endl;
                cout << "\t\t\t\t venue name: " << item.generalProperties.venueSnapshotName << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetMemoryLocations Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetMemoryLocations returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
