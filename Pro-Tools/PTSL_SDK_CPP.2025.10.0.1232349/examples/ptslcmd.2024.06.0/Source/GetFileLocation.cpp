// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetFileLocation command.
 */

#include "Common.h"

const std::string g_pszGetFileLocation = "GetFileLocation";
const std::string g_pszGetFileLocationHelp =
    "GetFileLocation [-limit <limit>] [-offset <offset>] [-file_filters {All_Files|OnTimeline_Files|NotOnTimeline_Files|Online_Files|Offline_Files|Audio_Files|Video_Files|Rendered_Files|SelectedClipsTimeline|SelectedClipsClipsList}]";

PtslCmdCommandResult GetFileLocation(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    GetFileLocationRequest request = {};
    request.commandType = CommandType::GetFileLocation;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        request.paginationRequest.limit = 0;
        request.paginationRequest.offset = 0;

        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        // Populate the request by the parameters and their args provided:
        for (const auto& pair : paramsArgsMap)
        {
            const string param = pair.first;
            const vector<string> args = pair.second;

            if (param == "limit")
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " "
                         << "limit" << endl;
                    return false;
                }

                request.paginationRequest.limit = stoi(args.at(0));
            }
            else if (param == "offset")
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " "
                         << "offset" << endl;
                    return false;
                }

                request.paginationRequest.offset = stoi(args.at(0));
            }
            else if (param == "file_filters")
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " "
                         << "file_filters" << endl;
                    return false;
                }

                map<string, FileLocationTypeFilter> fileLocationTypeFilterMap = { RVRS_MAP_ENTRY(FileLocationTypeFilter,
                                                                                      All_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, OnTimeline_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, NotOnTimeline_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, Online_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, Offline_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, Audio_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, Video_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, Rendered_Files),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, SelectedClipsTimeline),
                    RVRS_MAP_ENTRY(FileLocationTypeFilter, SelectedClipsClipsList) };

                for (const auto& theArg : args)
                {
                    if (fileLocationTypeFilterMap.count(theArg) == 0)
                    {
                        cout << CommandLineParser::PARAMETER_WRONG_USAGE << " "
                             << "file_filters"
                             << ". There is no such an argument: " << theArg << endl;
                        return false;
                    }

                    request.fileFilters.push_back(fileLocationTypeFilterMap.at(theArg));
                }
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    std::shared_ptr<GetFileLocationResponse> rsp = client.GetFileLocation(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetFileLocation Response:" << endl;

        if (rsp->fileLocations.size())
        {
            for (auto& fileLoc : rsp->fileLocations)
            {
                cout << "  " << fileLoc.path << "  " << (fileLoc.info.isOnline ? "online" : "offline") << endl;
            }
        }
        else
        {
            cout << "  --No File Locations Returned--" << endl;
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetFileLocation Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetFileLocation Request return an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
