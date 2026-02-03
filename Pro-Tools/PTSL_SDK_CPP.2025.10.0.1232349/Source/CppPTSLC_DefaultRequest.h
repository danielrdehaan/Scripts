// Copyright 2021-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Base functionality for PTSL request handlers.
 */

#pragma once

#include "CppPTSLClient.h"
#include "CppPTSLClientInternal.h"
#include <algorithm>
#include <string>
#include <tuple>
#include <utility>

// Deprecated starting in 2024.10
#define INIT_HNDLR(CMD)                                                                                                \
    CMD##Handler()                                                                                                     \
    {                                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    CMD##Handler(const CommandRequest& request)                                                                        \
    {                                                                                                                  \
    }

// Indeed, parsing `mGrpcRequestBody` from JSON to convert it later back to JSON may seem peculiar.
// However, this method ensures a consistent process for all scenarios further on, whether it's JSON or command line.

// Deprecated starting in 2024.10
//  Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function,
//  so you don't need JSON <-> message conversion.
#define INIT_HNDLR_OVRD(CMD)                                                                                           \
    CMD##Handler()                                                                                                     \
    {                                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    CMD##Handler(const CMD##Request& request)                                                                          \
    {                                                                                                                  \
        std::string jsonBody = request.directJsonBody;                                                                 \
        if (!jsonBody.empty() && !std::all_of(jsonBody.begin(), jsonBody.end(), isspace))                              \
        {                                                                                                              \
            google::protobuf::util::JsonStringToMessage(jsonBody, &mGrpcRequestBody);                                  \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            this->FillGrpcRequest(request);                                                                            \
        }                                                                                                              \
    }

// Deprecated starting in 2024.10
#define CREATE_STATE_OVRD(CMD)                                                                                         \
    void CreateState() override                                                                                        \
    {                                                                                                                  \
        mState = std::make_shared<CMD##State>();                                                                       \
    }

// Deprecated starting in 2024.10
#define MAKE_RESP_OVRD(CMD)                                                                                            \
    void MakeResponse() override                                                                                       \
    {                                                                                                                  \
        mResponse = std::make_shared<CMD##Response>();                                                                 \
    }


// Deprecated starting in 2024.10
//  Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function
//  instead of MAKE_REQUEST
#define MAKE_REQUEST(CMD, HAS_REQ, IS_STREAMING)                                                                       \
    std::shared_ptr<DefaultRequestHandler> hndlr;                                                                      \
                                                                                                                       \
    if (HAS_REQ)                                                                                                       \
        hndlr = std::make_shared<CMD##Handler>(request);                                                               \
    else                                                                                                               \
        hndlr = std::make_shared<CMD##Handler>();                                                                      \
                                                                                                                       \
    if (IS_STREAMING)                                                                                                  \
        this->MakeStreamingRequest(hndlr, CommandId::CId_##CMD);                                                       \
    else                                                                                                               \
        this->MakeRequest(hndlr, CommandId::CId_##CMD);

namespace PTSLC_CPP
{
    using namespace google::protobuf;
    using namespace google::protobuf::util;

    /**
     * Common request handler for processing specific grpc requests and responses.
     *
     * @deprecated Deprecated starting in Pro Tools 2024.10.
     * Please use a general JSON-based @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" function
     * instead of using request handlers.
     */
    struct [[deprecated("Please use PTSLC_CPP::CppPTSLClient::SendRequest()")]] DefaultRequestHandler
    {
    public:
        struct State
        {
            bool mNeedSendResponse = true;
        };

        DefaultRequestHandler() : mResponse(nullptr)
        {
        }

        virtual ~DefaultRequestHandler()
        {
        }

        bool ConvertRequestBodyToJson(std::string& requestBodyJSON)
        {
            bool status = false;

            JsonOptions jOpts = DefaultJsonWriteOptions();

            status = MessageToJsonString(GetRequestBodyRef(), &requestBodyJSON, jOpts).ok();
            return status;
        }

        std::tuple<bool, std::string> ConvertJsonToResponseBody(const std::string& responseBodyJSON)
        {
            JsonParseOptions jOpts = DefaultJsonParseOptions();
            auto status = JsonStringToMessage(responseBodyJSON, &GetResponseBodyRef(), jOpts);

            bool result = status.ok();
            std::string result_str = status.ToString();

            return { result, result_str };
        }

        std::tuple<bool, std::string> ConvertJsonToResponseError(const std::string& errorJSON)
        {
            JsonParseOptions jOpts = DefaultJsonParseOptions();

            auto status = JsonStringToMessage(errorJSON, &mGrpcResponseError, jOpts);
            bool result = status.ok();
            std::string result_str = status.ToString();

            if (!status.ok())
            {
                ptsl::CommandError legacyError;
                auto legacy_err_status = JsonStringToMessage(errorJSON, &legacyError);
                result = legacy_err_status.ok();
                if (legacy_err_status.ok())
                {
                    mGrpcResponseError.add_errors()->CopyFrom(legacyError);
                    // Reset result_str to the legacy error status since
                    // the previous error was expected
                    result_str = legacy_err_status.ToString();
                }
                else
                {
                    // Append the legacy error status to the result string
                    // for additional context
                    result_str += ";" + legacy_err_status.ToString();
                }
            }

            return { result, result_str };
        }

        void ConvertProtoToCommandError(const ptsl::CommandError& in, CommandError& out)
        {
            out.errorType = static_cast<CommandErrorType>(in.command_error_type());
            out.errorMessage = in.command_error_message();
            out.isWarning = in.is_warning();
        }

        virtual void OnNextResponse()
        {
        }

        virtual void OnHasBody()
        {
        }

        virtual void OnNoBody()
        {
        }

        virtual bool IsResponseReceived() const
        {
            return true;
        }

        virtual void OnHasHeader(const ptsl::Response& response)
        {
            if (mResponse)
            {
                mResponse->header.commandType = (CommandId)response.header().command();
                mResponse->header.taskId = response.header().task_id();
                mResponse->header.version = response.header().version();
                mResponse->header.versionMinor = response.header().version_minor();
                mResponse->header.versionRevision = response.header().version_revision();
                mResponse->status.type = (TaskStatus)response.header().status();
                mResponse->status.progress = response.header().progress();
                mResponse->jsonBody = response.response_body_json();
                mResponse->jsonError = response.response_error_json();
            }
        }

        virtual void OnHasError()
        {
            if (mResponse)
            {
                for (const auto& error : mGrpcResponseError.errors())
                {
                    auto commandError = std::make_shared<CommandError>();
                    ConvertProtoToCommandError(error, *commandError);
                    mResponse->errors.push_back(commandError);
                }
            }
        }

        virtual void OnNoResponse(CommandId command)
        {
            if (mResponse)
            {
                mResponse->header.commandType = command;
                mResponse->status.type = TaskStatus::TStatus_NoResponseReceived;
            }
        }

        virtual void SetResponseStatus(const TaskStatus& status, const std::string& statusTypeClarification = "")
        {
            if (mResponse)
            {
                mResponse->status.type = status;
                mResponse->status.typeClarification = statusTypeClarification;
            }
        }

        virtual TaskStatus GetResponseStatus() const
        {
            TaskStatus status = TaskStatus::TStatus_NoResponseReceived;

            if (mResponse)
            {
                status = mResponse->status.type;
            }

            return status;
        }

        virtual bool IsNeedToPingTaskStatus() const
        {
            return false;
        }

        virtual std::string GetRequestName() const = 0;

        virtual void CreateState()
        {
            mState = std::make_shared<State>();
        }

        virtual void MakeResponse()
        {
            mResponse = std::make_shared<CommandResponse>();
        }

        virtual std::shared_ptr<CommandResponse> GetResponse() const
        {
            return mResponse;
        }

    protected:
        virtual google::protobuf::Message& GetRequestBodyRef()
        {
            return sEmptyMessage;
        }

        virtual google::protobuf::Message& GetResponseBodyRef()
        {
            return sEmptyMessage;
        }

        ptsl::ResponseError mGrpcResponseError;
        std::shared_ptr<State> mState;
        std::shared_ptr<CommandResponse> mResponse;

    private:
        static google::protobuf::Message& sEmptyMessage;
    };
}; // namespace PTSLC_CPP
