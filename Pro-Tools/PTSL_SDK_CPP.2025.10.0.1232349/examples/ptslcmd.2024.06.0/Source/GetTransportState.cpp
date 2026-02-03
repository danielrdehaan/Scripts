// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetTransportState command.
 */

#include "Common.h"

const std::string g_pszGetTransportState = "GetTransportState";
const std::string g_pszGetTransportStateHelp = g_pszGetTransportState;

PtslCmdCommandResult GetTransportState(const std::vector<std::string>& params, CppPTSLClient& client)
{
    CommandRequest request;
    request.commandType = CommandType::GetTransportState;

    // Call the client's method with the created request:
    std::shared_ptr<GetTransportStateResponse> rsp = client.GetTransportState(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    const std::map<TS_TransportState, string> enumMap = {
        MAP_ENTRY(TS_TransportState, TS_TransportPlaying),
        MAP_ENTRY(TS_TransportState, TS_TransportStopped),
        MAP_ENTRY(TS_TransportState, TS_TransportRecording),
        MAP_ENTRY(TS_TransportState, TS_TransportPlayingHalfSpeed),
        MAP_ENTRY(TS_TransportState, TS_TransportRecordingHalfSpeed),
        MAP_ENTRY(TS_TransportState, TS_TransportFastForward),
        MAP_ENTRY(TS_TransportState, TS_TransportRewind),
        MAP_ENTRY(TS_TransportState, TS_TransportScrub),
        MAP_ENTRY(TS_TransportState, TS_TransportShuttle),
        MAP_ENTRY(TS_TransportState, TS_TransportPrimed),
        MAP_ENTRY(TS_TransportState, TS_TransportIsCueing),
        MAP_ENTRY(TS_TransportState, TS_TransportIsCuedForPreview),
        MAP_ENTRY(TS_TransportState, TS_TransportIsStopping),
        MAP_ENTRY(TS_TransportState, TS_TransportIsPreviewing),
    };

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetTransportState Response:" << endl;
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
        cout << "GetTransportState Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetTransportState returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
