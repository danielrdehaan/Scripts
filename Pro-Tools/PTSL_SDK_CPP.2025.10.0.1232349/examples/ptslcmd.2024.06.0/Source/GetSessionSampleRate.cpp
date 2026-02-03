// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionSampleRate command.
 */

#include "Common.h"

const std::string g_pszGetSessionSampleRate = "GetSessionSampleRate";
const std::string g_pszGetSessionSampleRateHelp = g_pszGetSessionSampleRate;

PtslCmdCommandResult GetSessionSampleRate(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionSampleRate;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionSampleRateResponse> rsp = client.GetSessionSampleRate(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<SampleRate, string> enumMap = {
        MAP_ENTRY(SampleRate, SR_44100),
        MAP_ENTRY(SampleRate, SR_48000),
        MAP_ENTRY(SampleRate, SR_88200),
        MAP_ENTRY(SampleRate, SR_96000),
        MAP_ENTRY(SampleRate, SR_176400),
        MAP_ENTRY(SampleRate, SR_192000),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionSampleRate Response:" << endl;
        cout << "\t"
             << "sample rate:"
             << "\t" << (enumMap.count(rsp->sampleRate) > 0 ? enumMap.at(rsp->sampleRate) : "") << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionSampleRate Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionSampleRate returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
