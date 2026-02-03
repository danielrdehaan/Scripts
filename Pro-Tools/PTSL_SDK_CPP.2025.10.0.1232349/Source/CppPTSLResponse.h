// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief class that describes Pro Tools response and common data of response message.
 *
 * See CppPTSLResponse.cpp to view all initialization details.
 */

#pragma once

#include "CppPTSLCommon.h"
#include "PtslCCppExport.h"

namespace PTSLC_CPP
{
    class PTSLC_CPP_EXPORT CppPTSLResponse
    {
    public:
        CppPTSLResponse() = default;
        CppPTSLResponse(
            CommandId commandId, const std::string& responseBodyJson = "", const std::string& responseErrorJson = "");

        std::string GetTaskId() const;
        void SetTaskId(const std::string& taskId);

        CommandId GetCommandId() const;
        void SetCommandId(const CommandId& commandId);

        CommandStatusType GetStatus() const;
        void SetStatus(const CommandStatusType& status);

        int32_t GetProgress() const;
        void SetProgress(const int32_t progress);

        std::string GetResponseBodyJson() const;
        void SetResponseBodyJson(const std::string& responseBodyJson);

        std::string GetResponseErrorJson() const;
        void SetResponseErrorJson(const std::string& responseErrorJson);

        ResponseError GetResponseErrorList() const;

        int32_t GetVersion() const;
        void SetVersion(const int32_t version);

        int32_t GetVersionMinor() const;
        void SetVersionMinor(const int32_t versionMinor);

        int32_t GetVersionRevision() const;
        void SetVersionRevision(const int32_t versionRevision);

        std::string GetVersionedResponseHeaderJson() const;
        void SetVersionedResponseHeaderJson(const std::string& versionedRequestHeaderJson);

    private:
        void ParseResponseError();

        // data of response header
        std::string mTaskId = "";
        CommandId mCommandId = CommandId::CId_None;
        CommandStatusType mStatus = CommandStatusType::TStatus_Failed;
        int32_t mProgress = 0;
        int32_t mVersion = 0;
        int32_t mVersionMinor = 0;
        int32_t mVersionRevision = 0;
        std::string mVersionedResponseHeaderJson = "";

        // data of response body
        std::string mResponseBodyJson = "";
        std::string mResponseErrorJson = "";

        // parsed data of response error
        ResponseError mResponseErrorList;
    };
} // namespace PTSLC_CPP
