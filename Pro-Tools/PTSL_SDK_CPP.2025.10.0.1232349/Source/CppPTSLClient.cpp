// Copyright 2020-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation file for the CppPTSLClient.h
 */

#include "CppPTSLClient.h"
#include "CppPTSLClientInternal.h"

#if defined(_WIN32)
#include <shellapi.h>
#include <Windows.h>
#elif defined(__APPLE__)
#include <dlfcn.h>
#endif

#include <algorithm>
#include <nlohmann/json.hpp>
#include <optional>

using json = nlohmann::json;

namespace PTSLC_CPP
{
    const int32_t PING_TIMEOUT = 5000; // 5 sec in milliseconds;

    /**
    * Client constructor. Used for initialization of gRPC Client and client's config.
    */
    CppPTSLClient::CppPTSLClient(const ClientConfig& config)
        : m_isHostReady(false),
          m_clientConfig(config),
          m_internalData(std::make_unique<InternalData>())
    {
        grpc::ChannelArguments channelArgs;
        channelArgs.SetInt(GRPC_ARG_MAX_RECEIVE_MESSAGE_LENGTH, std::numeric_limits<int32_t>::max());

        m_internalData->m_client = ptsl::PTSL::NewStub(
            grpc::CreateCustomChannel(config.address, grpc::InsecureChannelCredentials(), channelArgs));

        this->Init();
    }

    /**
    * Client destructor.
    */
    CppPTSLClient::~CppPTSLClient()
    {
        m_isHostReady = false;

        CancelRequests();

        m_internalData->m_completionQueue.Shutdown();
        this->Free();
    }

    /**
    * Launches Pro Tools application.
    */
    bool CppPTSLClient::LaunchProTools()
    {
#if defined(_WIN32)
        return FindWindow(L"DigiAppWndClass", L"Pro Tools") || ShellExecute(0, L"open", L"ProTools.exe", 0, 0, SW_SHOW);
#elif defined(__APPLE__)
        std::string command = "open -g -a 'Pro Tools'";
        system(command.c_str());
        return false;
#endif
    }

    std::string CppPTSLClient::GetSessionId() const
    {
        std::lock_guard m_sessionIdLock(CppPTSLClient::SessionIdMutex);
        return SessionId;
    }

    void CppPTSLClient::SetSessionId(const std::string& sessionId)
    {
        std::lock_guard m_sessionIdLock(CppPTSLClient::SessionIdMutex);
        SessionId = sessionId;
    }

    /**
    * Used for initialization client, starting PT and host (if required) and checking if host ready.
    */
    void CppPTSLClient::Init()
    {
        if (m_clientConfig.skipHostLaunch != SkipHostLaunch::Yes)
        {
            this->LaunchProTools();
        }

        if (m_clientConfig.serverMode == Mode::Standalone)
        {
            this->LocateServer();
        }

        this->HostReadyCheck();
    }

    /**
    * Used to free up client resources.
    */
    void CppPTSLClient::Free()
    {
        if (m_clientConfig.serverMode == Mode::Standalone)
        {
#if defined(_WIN32)
            if (m_internalData->m_winHandle)
            {
                FreeLibrary(m_internalData->m_winHandle);
            }
#elif defined(__APPLE__)
            if (m_internalData->m_macHandle)
            {
                dlclose(m_internalData->m_macHandle);
            }
#endif
        }
    }

    /**
    * Internal client wrapper utility function called by the constructor, @ref PTSLC_CPP::CppPTSLClient::CppPTSLClient
    * and checks to see if the Pro Tools application is ready to execute PTSL commands.
    */
    void CppPTSLClient::HostReadyCheck()
    {
        using namespace google::protobuf::util;

        CppPTSLRequest request { CommandId::CId_HostReadyCheck };

        auto resp = this->SendRequest(request).get();

        // TODO: Explicit error handling

        // Legacy response handling
        if (resp.GetResponseBodyJson().empty())
        {
            this->m_isHostReady = resp.GetStatus() == CommandStatusType::Completed;
        }
        // Current response handling
        else
        {
            ptsl::HostReadyCheckResponseBody grpcResponseBody;
            JsonParseOptions jOpts = DefaultJsonParseOptions();
            if (JsonStringToMessage(resp.GetResponseBodyJson(), &grpcResponseBody, jOpts).ok())
            {
                this->m_isHostReady = grpcResponseBody.is_host_ready();
            }
            else
            {
                this->m_isHostReady = false;
            }
        }
    }

    /**
    * Starts the server. Use this method only for unit tests.
    * In real use, PTSL.dll will load together during Pro Tools launch.
    * If you want to debug the server via client and launch server separately,
    * you need to comment calls of this method in the client constructor.
    */
    bool CppPTSLClient::LocateServer()
    {
        return true; // REMOVE ME: To load the host wrapper directly!

#if defined(_WIN32)
        if (!m_internalData->m_winHandle)
        {
#ifdef _DEBUG
            m_internalData->m_winHandle = LoadLibrary(L"..\\..\\..\\..\\..\\WinBag\\x64\\Debug\\bin\\PTSL.dll");
#else
            m_internalData->m_winHandle = LoadLibrary(L"..\\..\\..\\..\\..\\WinBag\\x64\\Release\\bin\\PTSL.dll");
#endif
        }

        return m_internalData->m_winHandle != nullptr;

#elif defined(__APPLE__)

        if (!m_internalData->m_macHandle)
        {
#ifdef _DEBUG
            m_internalData->m_macHandle = dlopen("../../../../../MacBag/Debug/libPTSL.dylib", RTLD_LOCAL);
#else
            m_internalData->m_macHandle = dlopen("../../../../../MacBag/Release/libPTSL.dylib", RTLD_LOCAL);
#endif
        }

        return m_internalData->m_macHandle != nullptr;
#endif
    }

    /**
     * Internal client wrapper feature.
     * Periodically checks the status of command execution with PING_TIMEOUT timeout.
     * Called right after gRPC request sent to the server and terminate command execution
     * if status of GetTaskStatus command execution is 'Failed'.
     */
    void CppPTSLClient::PingTaskStatus(std::string responseReceivedTaskId, std::future<void> pingCancellationToken)
    {
        using namespace google::protobuf::util;

        try
        {
            // wait 5 seconds before start pinging
            AsyncDelay(pingCancellationToken, std::chrono::milliseconds(PING_TIMEOUT));

            while (!IsCancellationRequested(pingCancellationToken))
            {
                ptsl::GetTaskStatusRequestBody grpcRequestBody;
                grpcRequestBody.set_task_id(responseReceivedTaskId);

                JsonOptions jOpts = DefaultJsonWriteOptions();

                std::string requestBodyJSON;
                MessageToJsonString(grpcRequestBody, &requestBodyJSON, jOpts);

                ptsl::Request grpcRequest;
                ptsl::Response grpcResponse;

                grpcRequest.mutable_header()->set_task_id("");
                grpcRequest.mutable_header()->set_session_id("");
                grpcRequest.mutable_header()->set_command(ptsl::CommandId::GetTaskStatus);
                grpcRequest.mutable_header()->set_version(PTSL_VERSION_MAJOR);
                grpcRequest.mutable_header()->set_version_minor(PTSL_VERSION_MINOR);
                grpcRequest.mutable_header()->set_version_revision(PTSL_VERSION_REVISION);
                grpcRequest.set_request_body_json(requestBodyJSON);

                grpc::Status grpcStatus;
                grpc::ClientContext context;
                grpc::CompletionQueue completionQueue;

                auto asyncReader =
                    m_internalData->m_client->AsyncSendGrpcRequest(&context, grpcRequest, &completionQueue);
                asyncReader->Finish(&grpcResponse, &grpcStatus, (void*)true);

                void* gotTag;
                bool canReadNext;

                bool isNextResponseExist = completionQueue.Next(&gotTag, &canReadNext);

                if (isNextResponseExist && canReadNext && gotTag == (void*)true && grpcStatus.ok())
                {
                    if (grpcResponse.header().status() == ptsl::TaskStatus::Completed)
                    {
                        completionQueue.Shutdown();
                        return;
                    }

                    if (grpcResponse.header().status() == ptsl::TaskStatus::CompletedWithBadResponse
                        || grpcResponse.header().status() == ptsl::TaskStatus::FailedWithBadErrorResponse)
                    {
                        completionQueue.Shutdown();
                        static const std::string warningMessage(
                            "PingTaskStatus canceled. GetTaskStatus returned incorrect response JSON. Status: "
                            + std::to_string(grpcResponse.header().status()));
                        SafeLogger::SyncPrint(warningMessage);
                        return;
                    }

                    if (grpcResponse.header().status() == ptsl::TaskStatus::Failed)
                    {
                        completionQueue.Shutdown();
                        static const std::string errorMessage(
                            "PingTaskStatus canceled. GetTaskStatus returned status Failed. Could not find a task with task ID: "
                            + responseReceivedTaskId);
                        throw std::runtime_error(errorMessage.c_str());
                    }
                }

                completionQueue.Shutdown();
                AsyncDelay(pingCancellationToken, std::chrono::milliseconds(PING_TIMEOUT));
            }
        }
        catch (const TaskCancelledException&)
        {
            return;
        }
    }

    std::shared_ptr<CommandResponse> CppPTSLClient::SendErrorResponse(CommandId commandType)
    {
        auto response = std::make_shared<CommandResponse>();

        response->header.commandType = commandType;
        response->status.type = TaskStatus::TStatus_Failed;
        auto error = std::make_shared<CommandError>();
        error->errorType = CommandErrorType::CEType_OS_ProToolsIsNotAvailable;
        error->errorMessage = "[Error]: Pro Tools is not available yet.";
        response->errors.push_back(error);

        if (this->OnResponseReceived_V1_Deprecated)
        {
            this->OnResponseReceived_V1_Deprecated(response);
        }

        return response;
    }

    CppPTSLResponse CppPTSLClient::SendErrorResponse(
        CommandType commandType, CommandErrorType errorType, const std::string& errorMessage)
    {
        using namespace google::protobuf::util;

        auto response = CppPTSLResponse { commandType };

        response.SetStatus(CommandStatusType::Failed);
        ptsl::ResponseError responseError;
        ptsl::CommandError* error = responseError.add_errors();
        error->set_command_error_type(static_cast<ptsl::CommandErrorType>(errorType));
        error->set_command_error_message(errorMessage);

        JsonOptions jOpts = DefaultJsonWriteOptions();
        std::string errorJson;

        if (MessageToJsonString(responseError, &errorJson, jOpts).ok())
        {
            response.SetResponseErrorJson(errorJson);
        }

        if (this->OnResponseReceived)
        {
            this->OnResponseReceived(std::make_shared<CppPTSLResponse>(response));
        }

        return response;
    }

    CppPTSLResponse CppPTSLClient::SendHostNotReadyResponse(CommandType commandType)
    {
        return SendErrorResponse(commandType,
            CommandErrorType::CEType_OS_ProToolsIsNotAvailable,
            "[Error]: Pro Tools is not available yet.");
    }

    std::string CppPTSLClient::LookForSessionId(const std::string& responseBodyJson)
    {
        const std::string sessionIdKey = "session_id";
        std::string errorMessage {};

        // Quickest pre-check:
        if (responseBodyJson.empty() || responseBodyJson.find(sessionIdKey) == std::string::npos)
        {
            errorMessage = "Session ID not found in response body.";
        }

        try
        {
            auto parsedJson = json::parse(responseBodyJson);

            if (parsedJson.contains(sessionIdKey) && parsedJson[sessionIdKey].is_string())
            {
                return parsedJson[sessionIdKey].get<std::string>();
            }
            else
            {
                errorMessage = "Session ID not found in response body.";
            }
        }
        catch (json::exception& e)
        {
            errorMessage = std::string { "Exception while parsing session_id from response body: " } + e.what();
        }

        throw PTSLException(errorMessage + " Response body: " + responseBodyJson);

        return std::string();
    }

    std::future<CppPTSLResponse> CppPTSLClient::SendRequest(CppPTSLRequest request, std::function<void(const CppPTSLResponse&)> responseCallback)
    {
        auto context = std::make_shared<InternalData::RpcContext>();

        const auto commandId = request.GetCommandId();

        {
            std::lock_guard<std::mutex> lock(m_internalData->m_rpcContextsMutex);
            m_internalData->m_rpcContexts.push_back(context);
        }

        // Passing a weak pointer to the RPC call to detect when the RPC is finished.

        auto processRequest = [this, &grpcContext = context->m_grpcContext, request = std::move(request), responseCallback = std::move(responseCallback)]() -> CppPTSLResponse
        {
            // allows HostReadyCheck command to check if the Pro Tools application is fully loaded
            // and ready to execute all other PTSL commands
            if (!m_isHostReady && request.GetCommandId() != CommandType::HostReadyCheck)
            {
                auto notReadyResponse = this->SendHostNotReadyResponse(request.GetCommandId());
                return notReadyResponse;
            }

            ptsl::Request grpcRequest;
            ptsl::Response grpcResponse;
            CppPTSLResponse response { request.GetCommandId() };

            grpcRequest.mutable_header()->set_command(static_cast<ptsl::CommandId>(request.GetCommandId()));
            grpcRequest.mutable_header()->set_session_id(
                request.GetSessionId().empty() ? GetSessionId() : request.GetSessionId());
            grpcRequest.mutable_header()->set_version(request.GetVersion() == 0 ? PTSL_VERSION_MAJOR : request.GetVersion());
            grpcRequest.mutable_header()->set_version_minor(request.GetVersionMinor() == 0 ? PTSL_VERSION_MINOR : request.GetVersionMinor());
            grpcRequest.mutable_header()->set_version_revision(request.GetVersionRevision() == 0 ? PTSL_VERSION_REVISION : request.GetVersionRevision());
            grpcRequest.mutable_header()->set_versioned_request_header_json(request.GetVersionedRequestHeaderJson());

            grpcRequest.set_request_body_json(request.GetRequestBodyJson());

            auto reader = m_internalData->m_client->SendGrpcStreamingRequest(&grpcContext, grpcRequest);

            while (reader->Read(&grpcResponse))
            {
                response.SetCommandId(static_cast<CommandId>(grpcResponse.header().command()));
                response.SetTaskId(grpcResponse.header().task_id());
                response.SetStatus(static_cast<CommandStatusType>(grpcResponse.header().status()));
                response.SetVersion(grpcResponse.header().version());
                response.SetVersionMinor(grpcResponse.header().version_minor());
                response.SetVersionRevision(grpcResponse.header().version_revision());
                response.SetProgress(grpcResponse.header().progress());
                response.SetVersionedResponseHeaderJson(grpcResponse.header().versioned_response_header_json());
                response.SetResponseBodyJson(grpcResponse.response_body_json());
                response.SetResponseErrorJson(grpcResponse.response_error_json());

                if (responseCallback)
                {
                    responseCallback(response);
                }
            }

            grpc::Status grpcStatus = reader->Finish();

            if (grpcStatus.error_code())
            {
                return SendErrorResponse(request.GetCommandId(),
                    CommandErrorType::CEType_SDK_GrpcGeneric,
                    "PTSL request failed with grpc error code = " + std::to_string(grpcStatus.error_code()));
            }

            if (request.GetCommandId() == CommandId::RegisterConnection)
            {
                try
                {
                    this->SetSessionId(this->LookForSessionId(grpcResponse.response_body_json()));
                }
                catch (const PTSLException& e)
                {
                    return SendErrorResponse(
                        request.GetCommandId(), CommandErrorType::CEType_SDK_SessionIdParseError, e.what());
                }
            }

            if (this->OnResponseReceived)
            {
                this->OnResponseReceived(std::make_shared<CppPTSLResponse>(response));
            }

            return response;
        };

        auto processRequestWithCleanup = [this, commandId, processRequest = std::move(processRequest), weakContext = std::weak_ptr<InternalData::RpcContext>{context}]()
        {
            auto context = weakContext.lock();
            if (!context)
            {
                // The context has been cancelled or destroyed before the request was sent.
                CppPTSLResponse response{ commandId };
                response.SetStatus(CommandStatusType::Failed);
                return response;
            }

            CppPTSLResponse response = processRequest();

            // Remove the context from the list of active contexts.
            {
                std::lock_guard<std::mutex> lock(m_internalData->m_rpcContextsMutex);
                m_internalData->m_rpcContexts.remove(context);
            }

            return response;
        };

        std::future<CppPTSLResponse> fResponse = std::async(std::launch::async, processRequestWithCleanup);

        return fResponse;
    }

    void CppPTSLClient::CancelRequests(bool waitForCancel)
    {
        std::list<std::shared_ptr<InternalData::RpcContext>> contexts;
        {
            std::lock_guard<std::mutex> lock(m_internalData->m_rpcContextsMutex);
            contexts = m_internalData->m_rpcContexts;
            m_internalData->m_rpcContexts.clear();
        }

        // Cancel all grpc requests.
        for (auto& context : contexts)
        {
            // TryCancel is thread safe.
            context->m_grpcContext.TryCancel();
        }

        if (!waitForCancel)
        {
            return;
        }

        std::list<std::weak_ptr<InternalData::RpcContext>> weakContexts;
        for (auto& context : contexts)
        {
            weakContexts.push_back(context);
        }

        contexts.clear();

        // Wait for all contexts to be cleared.
        while (!weakContexts.empty())
        {
            weakContexts.remove_if([](const std::weak_ptr<InternalData::RpcContext>& weakContext) { return weakContext.lock() == nullptr; });
            std::this_thread::yield();
        }
    }

} // namespace PTSLC_CPP
