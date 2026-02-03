// Copyright 2020, 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RefreshTargetAudioFiles command.
 */

#include "Common.h"

const std::string g_pszRefreshTargetAudioFiles = "RefreshTargetAudioFiles";
const std::string g_pszRefreshTargetAudioFilesHelp = "RefreshTargetAudioFiles -file_list <file_list>";

PtslCmdCommandResult RefreshTargetAudioFiles(const std::vector<std::string>& params, CppPTSLClient& client)
{
    RefreshTargetAudioFilesRequest request;
    request.commandType = CommandType::RefreshTargetAudioFiles;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        size_t count = params.size();

        if (count > 1 && strcmp(params.at(0).c_str(), "-file_list") == 0)
        {

            auto paramsWithArgs = CommandLineParser::RetrieveParamsWithArgs(params);
            auto paramIter = paramsWithArgs.find("file_list");

            if (paramIter != paramsWithArgs.end())
            {
                size_t i = 0;
                for (const auto& file : paramIter->second)
                {
                    if (++i < count)
                    {
                        request.filesList.push_back(file);
                    }
                }
            }
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " file_list" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<RefreshTargetAudioFilesResponse> rsp = client.RefreshTargetAudioFiles(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "RefreshTargetAudioFiles Response:" << endl;

        cout << "\t"
             << "Count of successfully refreshed audio files: " << rsp->successCount << endl;
        cout << "\t"
             << "Count of failures during refreshing audio files: " << rsp->failureCount << endl;

        if (rsp->failureCount > 0)
        {
            cout << "\t"
                 << "Failures list: " << endl;
            for (const auto& failure : rsp->failureList)
            {
                cout << "\t\t" << failure << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "RefreshTargetAudioFiles Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << "\t" << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "RefreshTargetAudioFiles return an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
