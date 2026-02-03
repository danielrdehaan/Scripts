// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::GetPTSLVersion command.
 */

#include "Common.h"

const std::string g_pszGetPTSLVersion = "GetPTSLVersion";
const std::string g_pszGetPTSLVersionHelp = "GetPTSLVersion -ptsl_component {Host|Client|ClientAPI}";

PtslCmdCommandResult GetPTSLVersion(const std::vector<std::string>& params, CppPTSLClient& client)
{
    GetPTSLVersionRequest request;
    request.commandType = CommandType::GetPTSLVersion;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        if (params.size() > 1 && strcmp(params.at(0).c_str(), "-ptsl_component") == 0)
        {
            // filling in the parameters
            if (strcmp(params.at(1).c_str(), "Host") == 0)
            {
                request.ptslComponent = VersionedPTSLComponents::Host;
            }
            else if (strcmp(params.at(1).c_str(), "Client") == 0)
            {
                request.ptslComponent = VersionedPTSLComponents::Client;
            }
            else if (strcmp(params.at(1).c_str(), "ClientAPI") == 0)
            {
                request.ptslComponent = VersionedPTSLComponents::ClientAPI;
            }
        }
        else
        {
            cout << CommandLineParser::PARAMETER_MISSED << " ptsl_component" << endl;
            return false;
        }
    }

    // Call the client's method with the created request:
    std::shared_ptr<GetPTSLVersionResponse> rsp = client.GetPTSLVersion(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "GetPTSLVersion Response:" << endl;

        if (rsp->apiMethodsVersionsList.empty())
        {
            cout << "PTSL Host version:"
                 << "\t" << rsp->ptslHostVersion << endl;
        }
        else if (rsp->apiMethodsVersionsList.size() == 1)
        {
            cout << "PTSL Client version:"
                 << "\t" << rsp->apiMethodsVersionsList.at("Client") << endl;
        }
        else
        {
            cout << "PTSL Client API methods versions:" << endl;

            for (const auto& ver : rsp->apiMethodsVersionsList)
            {
                cout << "\t" << ver.first << ": \t" << ver.second << endl;
            }
        }
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "GetPTSLVersion Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "GetPTSLVersion returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
