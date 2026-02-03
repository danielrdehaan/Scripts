// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

#include <algorithm>
#include <nlohmann/json.hpp>
#include <optional>

#include "Constants.h"
#include "PTSLC_CPP/CppPTSLCommonConversions.h"
#include "PTSLC_CPP/CppPTSLResponse.h"

using json = nlohmann::json;

struct PtslCmdCommandResult
{
    bool gotResponse = false;
    std::string commandName = "";
    std::string jsonHeader = "";
    std::string jsonBody = "";
    std::string jsonError = "";
    std::optional<std::string> cannotParseArgsReason = std::nullopt;

    explicit PtslCmdCommandResult(const PTSLC_CPP::CppPTSLResponse& clientResponse)
        : gotResponse(true),
          jsonBody(clientResponse.GetResponseBodyJson()),
          jsonError(clientResponse.GetResponseErrorJson())
    {
        json headerJson;
        headerJson[Constants::RESPONSE_HEADER_TASK_ID] = clientResponse.GetTaskId();
        headerJson[Constants::RESPONSE_HEADER_COMMAND] = EnumToString(clientResponse.GetCommandId());
        headerJson[Constants::RESPONSE_HEADER_STATUS] = EnumToString(clientResponse.GetStatus());
        headerJson[Constants::RESPONSE_HEADER_PROGRESS] = clientResponse.GetProgress();
        headerJson[Constants::RESPONSE_HEADER_VERSION] = clientResponse.GetVersion();
        headerJson[Constants::RESPONSE_HEADER_VERSION_MINOR] = clientResponse.GetVersionMinor();
        headerJson[Constants::RESPONSE_HEADER_VERSION_REVISION] = clientResponse.GetVersionRevision();
        headerJson[Constants::RESPONSE_HEADER_VERSIONED_RESPONSE_HEADER_JSON] = clientResponse.GetVersionedResponseHeaderJson();

        jsonHeader = headerJson.dump(indent) + "\n";
    }

    static PtslCmdCommandResult CreateCannotParseArgsResult(const std::string& reason, const std::string& cmdName = "")
    {
        auto result = PtslCmdCommandResult();
        result.gotResponse = false;
        result.commandName = cmdName;
        result.cannotParseArgsReason = reason;
        return result;
    }

    virtual ~PtslCmdCommandResult() = default;

    virtual std::string ToString() const
    {
        if (!gotResponse)
        {
            return "";
        }

        auto isNullOrWhiteSpace = [](const std::string& str)
        { return str.empty() || std::all_of(str.begin(), str.end(), ::isspace); };

        json responseJson;
        auto emptyJson = json::object();

        responseJson[Constants::RESPONSE_HEADER] = isNullOrWhiteSpace(jsonHeader) ? emptyJson : json::parse(jsonHeader);
        responseJson[Constants::RESPONSE_BODY] = isNullOrWhiteSpace(jsonBody) ? emptyJson : json::parse(jsonBody);
        responseJson[Constants::RESPONSE_ERROR] = isNullOrWhiteSpace(jsonError) ? emptyJson : json::parse(jsonError);

        return responseJson.dump(indent) + "\n";
    }

    operator bool() const
    {
        return gotResponse;
    }

private:
    PtslCmdCommandResult() = default;

    inline static const int indent = 2;
};

struct PtslCmdCommandStreamResult
{
    std::vector<PtslCmdCommandResult> results;
};
