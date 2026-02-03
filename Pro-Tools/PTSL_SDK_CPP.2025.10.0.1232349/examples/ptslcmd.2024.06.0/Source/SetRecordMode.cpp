// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetRecordMode command.
 */

#include "Common.h"

const std::string g_pszSetRecordMode = "SetRecordMode";
const std::string g_pszSetRecordModeHelp =
    "SetRecordMode -record_mode {Normal|Loop|Destructive|QuickPunch|TrackPunch|DestructivePunch} [-no_transport_armed]";

PtslCmdCommandResult SetRecordMode(const std::vector<std::string>& params, CppPTSLClient& client)
{
    SetRecordModeRequest request;
    request.commandType = CommandType::SetRecordMode;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissed = true;

        request.recordMode = RM_RecordMode::RM_Normal;
        request.recordArmTransport = true;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-record_mode") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "Normal") == 0)
                    {
                        request.recordMode = RM_RecordMode::RM_Normal;
                    }
                    else if (strcmp(params.at(i).c_str(), "Loop") == 0)
                    {
                        request.recordMode = RM_RecordMode::RM_Loop;
                    }
                    else if (strcmp(params.at(i).c_str(), "Destructive") == 0)
                    {
                        request.recordMode = RM_RecordMode::RM_Destructive;
                    }
                    else if (strcmp(params.at(i).c_str(), "QuickPunch") == 0)
                    {
                        request.recordMode = RM_RecordMode::RM_QuickPunch;
                    }
                    else if (strcmp(params.at(i).c_str(), "TrackPunch") == 0)
                    {
                        request.recordMode = RM_RecordMode::RM_TrackPunch;
                    }
                    else if (strcmp(params.at(i).c_str(), "DestructivePunch") == 0)
                    {
                        request.recordMode = RM_RecordMode::RM_DestructivePunch;
                    }

                    bMissed = false;
                }
            }

            if (strcmp(params.at(i).c_str(), "-no_transport_armed") == 0)
            {
                request.recordArmTransport = false;
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " record_mode" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetRecordMode(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
