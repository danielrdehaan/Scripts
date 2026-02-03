// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

// Consider using a JSON library for this file (e.g., nlohmann/json).
#include "PTSLC_CPP/CppPTSLCommonConversions.h"
#include <algorithm>

struct PtslCmdCommandResult
{
    bool gotResponse;
    std::string jsonHeader;
    std::string jsonBody;
    std::string jsonError;

    // allowed to use implicitly:
    PtslCmdCommandResult(bool cmdArgsParsedSuccessfully)
        : gotResponse(cmdArgsParsedSuccessfully),
          jsonHeader(""),
          jsonBody(""),
          jsonError("")
    {
    }

    explicit PtslCmdCommandResult(const std::shared_ptr<PTSLC_CPP::CommandResponse>& clientResponse)
        : gotResponse(true),
          jsonBody(clientResponse->jsonBody),
          jsonError(clientResponse->jsonError)
    {
        jsonHeader = "{\n  \"taskId\": \"" + clientResponse->header.taskId + "\",\n  \"command\": \""
            + EnumToString(clientResponse->header.commandType) + "\",\n  \"status\": \""
            + EnumToString(clientResponse->status.type)
            + "\",\n  \"progress\": " + std::to_string(clientResponse->status.progress) + "\n}\n";
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

        auto addIndentation = [](const std::string& s) -> std::string
        {
            std::istringstream iss(s);
            std::ostringstream oss;
            std::string line;

            while (std::getline(iss, line))
            {
                oss << "  " << line << "\n";
            }

            return oss.str();
        };

        const std::string emptyJson = " {}\n";

        return "\n{\n  \"header\":" + (isNullOrWhiteSpace(jsonHeader) ? emptyJson : addIndentation(jsonHeader))
            + "  ,\"responseBodyJson\":" + (isNullOrWhiteSpace(jsonBody) ? emptyJson : addIndentation(jsonBody))
            + "  ,\"responseErrorJson\":" + (isNullOrWhiteSpace(jsonError) ? emptyJson : addIndentation(jsonError))
            + "}\n";
    }

    operator bool() const
    {
        return gotResponse;
    }
};
