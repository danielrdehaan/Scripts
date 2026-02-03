// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information
// of Avid. Do not disclose to any third party. Use of the
// information contained in this document is subject to an Avid
// SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::RegisterConnection command.
 */

#include "Common.h"

const std::string g_pszRegisterConnection = "RegisterConnection";
const std::string g_pszRegisterConnectionHelp =
    "RegisterConnection -company_name <company_name> -application_name <application_name>";

PtslCmdCommandResult RegisterConnection(const std::vector<std::string>& params, CppPTSLClient& client)
{
    // Create a request:
    RegisterConnectionRequest request;
    request.commandType = CommandType::RegisterConnection;

    if (!FillCommandRequestByDirectJson(params, /*out*/ request))
    // then parse `params` and fill the request manually:
    {
        const string companyNameParam = "company_name";
        const string applicationNameParam = "application_name";
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

            if (param == companyNameParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << companyNameParam << endl;
                    return false;
                }

                request.companyName = args.at(0);
            }
            else if (param == applicationNameParam)
            {
                if (args.empty())
                {
                    cout << CommandLineParser::PARAMETER_WRONG_USAGE << " " << applicationNameParam << endl;
                    return false;
                }

                request.applicationName = args.at(0);
            }
            else
            {
                cout << "Wrong parameter: " << param << endl;
                return false;
            }
        }
    }

    // Call the client's method with the request:
    std::shared_ptr<RegisterConnectionResponse> rsp = client.RegisterConnection(request);

    // Output the response:
    if (!rsp)
    {
        cout << CommandLineParser::NO_RESPONSE_RECEIVED << endl;
        return false;
    }

    if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
    {
        cout << "RegisterConnection Response:" << endl;
        cout << "\t"
             << "session id:"
             << "\t" << rsp->sessionId << endl;
    }
    else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
    {
        cout << "RegisterConnection Request Failed" << endl;
        for (const auto& error : rsp->errors)
        {
            cout << error->errorMessage << endl;
        }
    }
    else
    {
        cout << "RegisterConnection returned an unexpected status" << endl;
    }

    return PtslCmdCommandResult(rsp);
}
