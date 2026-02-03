// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#include <filesystem>
#include <fstream>
#include <optional>

#include "FileScriptParser.h"
#include "PTSLC_CPP/CppPTSLRequest.h"
#include "PTSLC_CPP/CppCryptoUtils.h"
#include "PtslCmdCommandRunner.h"

using namespace std;
using namespace PTSLC_CPP;

PtslCmdCommandStreamResult PtslCmdCommandRunner::RunCommand(const PtslCmdCommandRequest& ptslCmdRequest)
{
    const auto clientCommandId = StringToEnum<CommandId>(ptslCmdRequest.CommandName);
    if (!clientCommandId)
    {
        return { { PtslCmdCommandResult::CreateCannotParseArgsResult("Invalid command.", ptslCmdRequest.CommandName) } };
    }

    CppPTSLRequest clientRequest(*clientCommandId);

    if (!ptslCmdRequest.EncryptionKey.empty())
    {
        if (clientCommandId == CommandId::CId_EnableAPI)
        {
            json requestBody = json::parse(ptslCmdRequest.RequestBodyJson);
            const auto client_key_string_field = "client_key_string";
            const string original_client_key = requestBody[client_key_string_field].get<std::string>();
            const string b64string = encodeBase64(encrypt(original_client_key, ptslCmdRequest.EncryptionKey));
            requestBody[client_key_string_field] = b64string;
            clientRequest.SetRequestBodyJson(requestBody.dump());
        }
        else
        {
            std::cout << "Encryption is not applicable for CommandId other than CId_EnableAPI. Ignoring." << std::endl;
        }
    }
    else
    {
        clientRequest.SetRequestBodyJson(ptslCmdRequest.RequestBodyJson);
    }

    if (std::get<0>(ptslCmdRequest.Version) > 0)
    {
        clientRequest.SetVersion(std::get<0>(ptslCmdRequest.Version));
        clientRequest.SetVersionMinor(std::get<1>(ptslCmdRequest.Version));
        clientRequest.SetVersionRevision(std::get<2>(ptslCmdRequest.Version));
    }

    if (!ptslCmdRequest.VersionedHeader.empty())
    {
        clientRequest.SetVersionedRequestHeaderJson(ptslCmdRequest.VersionedHeader);
    }
    
    PtslCmdCommandStreamResult result;
    mClientPtr->SendRequest(clientRequest, [&](const CppPTSLResponse& response)
    {
        // No need to synchronize since the main thread is waiting for the result.
        result.results.push_back(PtslCmdCommandResult(response));
    }).get();

    return result;
}

vector<PtslCmdCommandStreamResult> PtslCmdCommandRunner::RunScript(const string& scriptJson)
{
    const optional<PtslCmdFileScript> script = FileScriptParser::Parse(scriptJson);

    if (!script)
    {
        return { { { PtslCmdCommandResult::CreateCannotParseArgsResult("Failed to parse the script") } } };
    }

    vector<PtslCmdCommandStreamResult> results;

    for (const auto& ptslCmdRequest : script->CommandRequests)
    {
        auto ptslCmdResult = this->RunCommand(ptslCmdRequest);
        results.push_back(ptslCmdResult);
    }

    return results;
}

vector<PtslCmdCommandStreamResult> PtslCmdCommandRunner::RunFileScript(const string& fileName)
{
    if (fileName.empty())
    {
        return { { { PtslCmdCommandResult::CreateCannotParseArgsResult("No file name provided") } } };
    }

    if (!filesystem::exists(fileName))
    {
        return { { { PtslCmdCommandResult::CreateCannotParseArgsResult("File not found", fileName) } } };
    }

    ifstream file(fileName);
    string scriptJson { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    file.close();

    return this->RunScript(scriptJson);
}
