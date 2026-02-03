// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation file for the CppPTSLResponse.h
 */

#include "CppPTSLResponse.h"
#include "CppPTSLClientInternal.h"

namespace PTSLC_CPP
{
    CppPTSLResponse::CppPTSLResponse(
        CommandId commandId, const std::string& responseBodyJson, const std::string& responseErrorJson)
        : mCommandId(commandId),
          mResponseBodyJson(responseBodyJson),
          mResponseErrorJson(responseErrorJson)
    {
    }

    std::string CppPTSLResponse::GetTaskId() const
    {
        return mTaskId;
    }

    void CppPTSLResponse::SetTaskId(const std::string& taskId)
    {
        mTaskId = taskId;
    }

    CommandId CppPTSLResponse::GetCommandId() const
    {
        return mCommandId;
    }

    void CppPTSLResponse::SetCommandId(const CommandId& commandId)
    {
        mCommandId = commandId;
    }

    CommandStatusType CppPTSLResponse::GetStatus() const
    {
        return mStatus;
    }

    void CppPTSLResponse::SetStatus(const CommandStatusType& status)
    {
        mStatus = status;
    }

    int32_t CppPTSLResponse::GetProgress() const
    {
        return mProgress;
    }

    void CppPTSLResponse::SetProgress(const int32_t progress)
    {
        mProgress = progress;
    }

    std::string CppPTSLResponse::GetResponseBodyJson() const
    {
        return mResponseBodyJson;
    }

    void CppPTSLResponse::SetResponseBodyJson(const std::string& responseBodyJson)
    {
        mResponseBodyJson = responseBodyJson;
    }

    std::string CppPTSLResponse::GetResponseErrorJson() const
    {
        return mResponseErrorJson;
    }

    void CppPTSLResponse::SetResponseErrorJson(const std::string& responseErrorJson)
    {
        mResponseErrorJson = responseErrorJson;
        ParseResponseError();
    }

    ResponseError CppPTSLResponse::GetResponseErrorList() const
    {
        return mResponseErrorList;
    }

    static void ConvertProtoToCommandError(const ptsl::CommandError& in, CommandError& out)
    {
        out.errorType = static_cast<CommandErrorType>(in.command_error_type());
        out.errorMessage = in.command_error_message();
        out.isWarning = in.is_warning();
    }

    void CppPTSLResponse::ParseResponseError()
    {
        using namespace google::protobuf::util;

        JsonParseOptions jOpts = DefaultJsonParseOptions();

        ptsl::ResponseError responseError;

        auto status = JsonStringToMessage(mResponseErrorJson, &responseError, jOpts);

        if (!status.ok())
        {
            ptsl::CommandError legacyError;
            auto legacy_err_status = JsonStringToMessage(mResponseErrorJson, &legacyError);
            if (legacy_err_status.ok())
            {
                responseError.add_errors()->CopyFrom(legacyError);
            }
        }

        for (const auto& error : responseError.errors())
        {
            auto commandError = std::make_shared<CommandError>();
            ConvertProtoToCommandError(error, *commandError);
            mResponseErrorList.errors.push_back(commandError);
        }
    }

    int32_t CppPTSLResponse::GetVersion() const
    {
        return mVersion;
    }

    void CppPTSLResponse::SetVersion(const int32_t version)
    {
        mVersion = version;
    }

    int32_t CppPTSLResponse::GetVersionMinor() const
    {
        return mVersionMinor;
    }

    void CppPTSLResponse::SetVersionMinor(const int32_t versionMinor)
    {
        mVersionMinor = versionMinor;
    }

    int32_t CppPTSLResponse::GetVersionRevision() const
    {
        return mVersionRevision;
    }

    void CppPTSLResponse::SetVersionRevision(const int32_t versionRevision)
    {
        mVersionRevision = versionRevision;
    }

    std::string CppPTSLResponse::GetVersionedResponseHeaderJson() const
    {
        return mVersionedResponseHeaderJson;
    }

    void CppPTSLResponse::SetVersionedResponseHeaderJson(const std::string& versionedRequestHeaderJson)
    {
        mVersionedResponseHeaderJson = versionedRequestHeaderJson;
    }

} // namespace PTSLC_CPP
