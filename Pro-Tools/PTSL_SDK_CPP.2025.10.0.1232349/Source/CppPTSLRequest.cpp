// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation file for the CppPTSLRequest.h
 */

#include "CppPTSLRequest.h"
#include "CppPTSLClient.h"

namespace PTSLC_CPP
{
    CppPTSLRequest::CppPTSLRequest(const CommandId commandId, const std::string& requestBodyJson)
        : mCommandId(commandId),
          mRequestBodyJson(requestBodyJson) {};

    CommandId CppPTSLRequest::GetCommandId() const
    {
        return mCommandId;
    }

    void CppPTSLRequest::SetCommandId(const CommandId& commandId)
    {
        mCommandId = commandId;
    }

    int32_t CppPTSLRequest::GetVersion() const
    {
        return mVersion;
    }

    void CppPTSLRequest::SetVersion(const int32_t version)
    {
        mVersion = version;
    }

    int32_t CppPTSLRequest::GetVersionMinor() const
    {
        return mVersionMinor;
    }

    void CppPTSLRequest::SetVersionMinor(const int32_t versionMinor)
    {
        mVersionMinor = versionMinor;
    }

    int32_t CppPTSLRequest::GetVersionRevision() const
    {
        return mVersionRevision;
    }

    void CppPTSLRequest::SetVersionRevision(const int32_t versionRevision)
    {
        mVersionRevision = versionRevision;
    }

    std::string CppPTSLRequest::GetSessionId() const
    {
        return mSessionId;
    }

    void CppPTSLRequest::SetSessionId(const std::string& sessionId)
    {
        mSessionId = sessionId;
    }

    std::string CppPTSLRequest::GetVersionedRequestHeaderJson() const
    {
        return mVersionedRequestHeaderJson;
    }

    void CppPTSLRequest::SetVersionedRequestHeaderJson(const std::string& versionedRequestHeaderJson)
    {
        mVersionedRequestHeaderJson = versionedRequestHeaderJson;
    }

    std::string CppPTSLRequest::GetRequestBodyJson() const
    {
        return mRequestBodyJson;
    }

    void CppPTSLRequest::SetRequestBodyJson(const std::string& requestBodyJson)
    {
        mRequestBodyJson = requestBodyJson;
    }
} // namespace PTSLC_CPP
