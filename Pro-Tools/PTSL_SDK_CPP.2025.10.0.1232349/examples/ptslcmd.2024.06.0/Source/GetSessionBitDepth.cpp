// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetSessionBitDepth command.
 */

#include "Common.h"

const std::string g_pszGetSessionBitDepth = "GetSessionBitDepth";
const std::string g_pszGetSessionBitDepthHelp = g_pszGetSessionBitDepth;

PtslCmdCommandResult GetSessionBitDepth(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetSessionBitDepth;

    // Call the client's method with the created request:
    std::shared_ptr<GetSessionBitDepthResponse> rsp = client.GetSessionBitDepth(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<BitDepth, string> enumMap = {
        MAP_ENTRY(BitDepth, Bit16),
        MAP_ENTRY(BitDepth, Bit24),
        MAP_ENTRY(BitDepth, Bit32Float),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetSessionBitDepth Response:" << endl;
        cout << "\t"
             << "current setting:"
             << "\t" << (enumMap.count(rsp->currentSetting) > 0 ? enumMap.at(rsp->currentSetting) : "") << endl;

        if (rsp->possibleSettings.size())
        {
            cout << "\t"
                 << "possible settings:" << endl;

            for (auto& item : rsp->possibleSettings)
            {
                cout << "\t\t" << (enumMap.count(item) > 0 ? enumMap.at(item) : "") << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetSessionBitDepth Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetSessionBitDepth returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
