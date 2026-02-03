// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::SetSessionBitDepth command.
 */

#include "Common.h"

const std::string g_pszSetSessionBitDepth = "SetSessionBitDepth";
const std::string g_pszSetSessionBitDepthHelp = "SetSessionBitDepth -bit_depth {Bit16|Bit24|Bit32Float}";

PtslCmdCommandResult SetSessionBitDepth(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    SetSessionBitDepthRequest request = {};
    request.commandType = CommandType::SetSessionBitDepth;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string bitDepthParam = "bit_depth";
        auto paramsArgsMap = CommandLineParser::RetrieveParamsWithArgs(params);

        // Populate the request by the parameters and their args provided:
        for (const auto& pair : paramsArgsMap)
        {
            const string param = pair.first;
            const vector<string> args = pair.second;

            if (param == CommandLineParser::PARAMETERLESS_ARGS_KEY && !args.empty())
            {
                cout << CommandLineParser::PARAMETERLESS_ARGS << " " << args.at(0) << endl;
                return false;
            }

            if (param == bitDepthParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << bitDepthParam << endl;
                    return false;
                }

                const string theArg = args.at(0);

                map<string, BitDepth> bitDepthMap = {
                    RVRS_MAP_ENTRY(BitDepth, Bit16),
                    RVRS_MAP_ENTRY(BitDepth, Bit24),
                    RVRS_MAP_ENTRY(BitDepth, Bit32Float),
                };

                if (bitDepthMap.count(theArg) == 0)
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << bitDepthParam
                         << ". There is no such an argument: " << theArg << endl;
                    return false;
                }

                request.bitDepth = bitDepthMap.at(theArg);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<CommandResponse> rsp = client.SetSessionBitDepth(request);

    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    return PtslCmdCommandResult(rsp);
}
