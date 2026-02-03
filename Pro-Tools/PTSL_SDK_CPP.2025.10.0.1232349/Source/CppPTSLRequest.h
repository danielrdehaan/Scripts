// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief class that describes Pro Tools request and common data of request message.
 *
 * See CppPTSLRequest.cpp to view all initialization details.
 */

#pragma once

#include "CppPTSLCommon.h"
#include "PtslCCppExport.h"

namespace PTSLC_CPP
{
    class PTSLC_CPP_EXPORT CppPTSLRequest
    {
    public:
        CppPTSLRequest() = delete;
        CppPTSLRequest(CommandId commandId, const std::string& requestBodyJson = "");

        CommandId GetCommandId() const;
        void SetCommandId(const CommandId& commandId);

        int32_t GetVersion() const;
        void SetVersion(const int32_t version);

        int32_t GetVersionMinor() const;
        void SetVersionMinor(const int32_t versionMinor);

        int32_t GetVersionRevision() const;
        void SetVersionRevision(const int32_t versionRevision);

        std::string GetSessionId() const;
        void SetSessionId(const std::string& sessionId);

        std::string GetVersionedRequestHeaderJson() const;
        void SetVersionedRequestHeaderJson(const std::string& versionedRequestHeaderJson);

        std::string GetRequestBodyJson() const;
        void SetRequestBodyJson(const std::string& requestBodyJson);
    private:
        // data of request header
        CommandId mCommandId = CommandId::CId_None;
        int32_t mVersion = 0;
        int32_t mVersionMinor = 0;
        int32_t mVersionRevision = 0;
        std::string mSessionId = "";
        std::string mVersionedRequestHeaderJson = "";

        // data of request body
        std::string mRequestBodyJson = "";
    };
} // namespace PTSLC_CPP
