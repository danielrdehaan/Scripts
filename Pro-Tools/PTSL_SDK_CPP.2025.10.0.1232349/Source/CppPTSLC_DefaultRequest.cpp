// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation file for the CppPTSLC_DefaultRequest.h
 */

#include "CppPTSLC_DefaultRequest.h"

namespace PTSLC_CPP
{
    using namespace google::protobuf;
    using namespace google::protobuf::util;

    static ptsl::EmptyMessage sTmpEmptyMessage = ptsl::EmptyMessage();
    google::protobuf::Message& DefaultRequestHandler::sEmptyMessage = sTmpEmptyMessage;

    /**
    * Internal common method for handling different types of requests and responses.
    */
    void CppPTSLClient::MakeRequest(std::shared_ptr<DefaultRequestHandler> handler, CommandId commandType)
    {
        // allows HostReadyCheck command to check if the Pro Tools application is fully loaded
        // and ready to execute all the others PTSL commands
        if (!m_isHostReady && commandType != CommandId::CId_HostReadyCheck)
        {
            this->SendErrorResponse(commandType);
            return;
        }

        try
        {
            auto callData = std::make_unique<CallData>();

            std::string requestBodyJson;

            handler->CreateState();

            // TODO: After removing deprecated CppPTSLC handlers, stop converting request bodies to json back and forth;
            //       pass direct_json_body directly to the server instead.
            handler->ConvertRequestBodyToJson(requestBodyJson);

            callData->m_grpcRequest.mutable_header()->set_task_id("");
            callData->m_grpcRequest.mutable_header()->set_session_id(SessionId);
            callData->m_grpcRequest.mutable_header()->set_command(static_cast<ptsl::CommandId>(commandType));
            callData->m_grpcRequest.mutable_header()->set_version(5);  // stick to the legacy version 2024.10
            callData->m_grpcRequest.mutable_header()->set_version_minor(0);
            callData->m_grpcRequest.mutable_header()->set_version_revision(0);
            callData->m_grpcRequest.set_request_body_json(requestBodyJson);

            auto asyncReader = m_internalData->m_client->AsyncSendGrpcRequest(
                &callData->m_context, callData->m_grpcRequest, &(m_internalData->m_completionQueue));
            asyncReader->Finish(&callData->m_grpcResponse, &callData->m_grpcStatus, callData.get());

            void* gotTag;
            bool canReadNext;

            bool isNextResponseExist = m_internalData->m_completionQueue.Next(&gotTag, &canReadNext);

            if (isNextResponseExist && canReadNext && gotTag == static_cast<void*>(callData.get()))
            {
                if (callData->m_grpcStatus.ok())
                {
                    handler->MakeResponse();
                    handler->OnNextResponse();

                    if (callData->m_grpcResponse.has_header())
                    {
                        // fill Header and Status
                        handler->OnHasHeader(callData->m_grpcResponse);

                        if (callData->m_grpcResponse.response_error_json().size())
                        {
                            // fill Error message
                            auto [isResultOk, statusTypeClarification] =
                                handler->ConvertJsonToResponseError(callData->m_grpcResponse.response_error_json());
                            if (!isResultOk)
                            {
                                handler->SetResponseStatus(
                                    TaskStatus::TStatus_FailedWithBadErrorResponse, statusTypeClarification);
                            }

                            handler->OnHasError();
                        }

                        if (callData->m_grpcResponse.response_body_json().size())
                        {
                            // fill Body fields
                            auto [isResultOk, statusTypeClarification] =
                                handler->ConvertJsonToResponseBody(callData->m_grpcResponse.response_body_json());
                            if (!isResultOk)
                            {
                                handler->SetResponseStatus(
                                    TaskStatus::TStatus_CompletedWithBadResponse, statusTypeClarification);
                            }

                            handler->OnHasBody();
                        }
                        else
                        {
                            handler->OnNoBody();
                        }
                    }
                    else
                    {
                        // empty response
                        handler->OnNoResponse(commandType);
                    }

                    // return response using callback if all data received
                    if (this->OnResponseReceived_V1_Deprecated && handler->IsResponseReceived())
                    {
                        this->OnResponseReceived_V1_Deprecated(handler->GetResponse());
                    }
                }
                else
                {
                    throw PTSLCommandException("Status = " + std::to_string(callData->m_grpcStatus.error_code()));
                }
            }
            else
            {
                throw PTSLCommandException();
            }
        }
        catch (const std::exception& ex)
        {
            auto response = std::make_shared<CommandResponse>();

            response->header.commandType = commandType;
            response->status.type = TaskStatus::TStatus_Failed;
            auto error = std::make_shared<CommandError>();
            error->errorType = CommandErrorType::SDK_Error;

            if (Is<PTSLCommandException>(&ex))
            {
                error->errorMessage = std::string() + "[Transport error]: " + handler->GetRequestName()
                    + " request failed. No response consists in the completion queue.";
            }
            else
            {
                error->errorMessage = "[Error]: " + std::string(ex.what());
            }

            response->errors.push_back(error);

            if (this->OnResponseReceived_V1_Deprecated)
            {
                this->OnResponseReceived_V1_Deprecated(response);
            }
        }
    }

    /**
    * Internal common method for handling different types of requests and responses.
    */
    void CppPTSLClient::MakeStreamingRequest(std::shared_ptr<DefaultRequestHandler> handler, CommandId commandType)
    {
        // allows HostReadyCheck command to check if the Pro Tools application is fully loaded
        // and ready to execute all the others PTSL commands
        if (!m_isHostReady && commandType != CommandId::CId_HostReadyCheck)
        {
            this->SendErrorResponse(commandType);
            return;
        }

        try
        {
            auto callData = std::make_unique<CallData>();

            std::string requestBodyJson;

            handler->CreateState();

            // TODO: After removing deprecated CppPTSLC handlers, stop converting request bodies to json back and forth;
            //       pass direct_json_body directly to the server instead.
            handler->ConvertRequestBodyToJson(requestBodyJson);

            callData->m_grpcRequest.mutable_header()->set_task_id("");
            callData->m_grpcRequest.mutable_header()->set_session_id(SessionId);
            callData->m_grpcRequest.mutable_header()->set_command(static_cast<ptsl::CommandId>(commandType));
            callData->m_grpcRequest.mutable_header()->set_version(5);  // stick to the legacy version 2024.10
            callData->m_grpcRequest.mutable_header()->set_version_minor(0);
            callData->m_grpcRequest.mutable_header()->set_version_revision(0);
            callData->m_grpcRequest.set_request_body_json(requestBodyJson);

            auto asyncStreamReader = m_internalData->m_client->AsyncSendGrpcStreamingRequest(
                &callData->m_context, callData->m_grpcRequest, &(m_internalData->m_completionQueue), callData.get());

            void* gotTag;
            bool canReadNext;

            bool isNextResponseExist = m_internalData->m_completionQueue.Next(&gotTag, &canReadNext);

            if (isNextResponseExist && canReadNext && gotTag == static_cast<void*>(callData.get()))
            {
                std::future<void> pingTask;
                std::promise<void> pingCancellationSignal;

                while (true)
                {
                    asyncStreamReader->Read(&callData->m_grpcResponse, callData.get());

                    // check if completion queue contains one more available response
                    canReadNext = false;
                    isNextResponseExist = m_internalData->m_completionQueue.Next(&gotTag, &canReadNext);

                    if (!isNextResponseExist || !canReadNext || gotTag != static_cast<void*>(callData.get()))
                    {
                        break;
                    }

                    handler->MakeResponse();
                    handler->OnNextResponse();

                    if (callData->m_grpcResponse.has_header())
                    {
                        // fill Header and Status
                        handler->OnHasHeader(callData->m_grpcResponse);

                        // ping task status if needed
                        if (handler->IsNeedToPingTaskStatus())
                        {
                            if (handler->GetResponseStatus() == TaskStatus::TStatus_Queued)
                            {
                                // start pinging current task execution status
                                std::future<void> pingCancellationToken = pingCancellationSignal.get_future();
                                pingTask = std::async(std::launch::async,
                                    &CppPTSLClient::PingTaskStatus,
                                    this,
                                    callData->m_grpcResponse.header().task_id(),
                                    std::move(pingCancellationToken));
                            }
                        }

                        if (!callData->m_grpcResponse.response_error_json().empty())
                        {
                            // fill Error message
                            auto [isResultOk, statusTypeClarification] =
                                handler->ConvertJsonToResponseError(callData->m_grpcResponse.response_error_json());
                            if (!isResultOk)
                            {
                                handler->SetResponseStatus(
                                    TaskStatus::TStatus_FailedWithBadErrorResponse, statusTypeClarification);
                            }

                            handler->OnHasError();
                        }

                        auto responseJson = callData->m_grpcResponse.response_body_json();
                        // TODO: Remove \n condition after fixing and testing all responses.
                        if (responseJson.size() && responseJson != "\n")
                        {
                            // fill Body fields
                            auto [isResultOk, statusTypeClarification] =
                                handler->ConvertJsonToResponseBody(responseJson);
                            if (!isResultOk)
                            {
                                handler->SetResponseStatus(
                                    TaskStatus::TStatus_CompletedWithBadResponse, statusTypeClarification);
                            }

                            handler->OnHasBody();
                        }
                        else
                        {
                            handler->OnNoBody();
                        }
                    }
                    else
                    {
                        // empty response
                        handler->OnNoResponse(commandType);
                    }

                    // return response using callback if all data received
                    if (this->OnResponseReceived_V1_Deprecated && handler->IsResponseReceived())
                    {
                        this->OnResponseReceived_V1_Deprecated(handler->GetResponse());
                    }

                    // ping task status if needed
                    if (handler->IsNeedToPingTaskStatus())
                    {
                        if (handler->GetResponseStatus() == TaskStatus::TStatus_Completed
                            || handler->GetResponseStatus() == TaskStatus::TStatus_CompletedWithBadResponse
                            || handler->GetResponseStatus() == TaskStatus::TStatus_FailedWithBadErrorResponse)
                        {
                            // cancel ping task
                            pingCancellationSignal.set_value();
                        }

                        if (IsCancellationRequested(pingTask))
                        {
                            // get results of the ping task if available (canceled or got exception)
                            pingTask.get();
                        }
                    }
                }
            }
            else
            {
                throw PTSLCommandException();
            }
        }
        catch (const std::exception& ex)
        {
            auto response = std::make_shared<CommandResponse>();

            response->header.commandType = commandType;
            response->status.type = TaskStatus::TStatus_Failed;
            auto error = std::make_shared<CommandError>();
            error->errorType = CommandErrorType::SDK_Error;

            if (Is<PTSLCommandException>(&ex))
            {
                error->errorMessage = std::string() + "[Transport error]: " + handler->GetRequestName()
                    + " request failed. No response consists in the completion queue.";
            }
            else
            {
                error->errorMessage = "[Error]: " + std::string(ex.what());
            }

            response->errors.push_back(error);

            if (this->OnResponseReceived_V1_Deprecated)
            {
                this->OnResponseReceived_V1_Deprecated(response);
            }
        }
    }
}; // namespace PTSLC_CPP
