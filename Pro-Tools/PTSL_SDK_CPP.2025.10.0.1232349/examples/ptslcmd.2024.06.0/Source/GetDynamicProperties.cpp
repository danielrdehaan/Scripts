// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetDynamicProperties command.
 */

#include "Common.h"

const std::string g_pszGetDynamicProperties = "GetDynamicProperties";
const std::string g_pszGetDynamicPropertiesHelp =
    "GetDynamicProperties -property_type {DP_EM_CodecInfo|DP_EM_DolbyAtmosInfo}";

PtslCmdCommandResult GetDynamicProperties(const std::vector<std::string>& params, CppPTSLClient& client)
{
    GetDynamicPropertiesRequest request;
    request.commandType = CommandType::GetDynamicProperties;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        bool bMissed = true;

        size_t count = params.size();

        for (int i = 0; i < count; ++i)
        {
            if (strcmp(params.at(i).c_str(), "-property_type") == 0)
            {
                if (++i < count)
                {
                    if (strcmp(params.at(i).c_str(), "DP_EM_CodecInfo") == 0)
                    {
                        request.propertyType = DynamicPropertyType::DP_EM_CodecInfo;
                    }
                    if (strcmp(params.at(i).c_str(), "DP_EM_DolbyAtmosInfo") == 0)
                    {
                        request.propertyType = DynamicPropertyType::DP_EM_DolbyAtmosInfo;
                    }

                    bMissed = false;
                }
            }
        }

        if (bMissed)
        {
            cout << CommandLineParser::PARAMETER_MISSED << " property_type" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.GetDynamicProperties(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
