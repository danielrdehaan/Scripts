// Copyright 2022-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Part of the CppPTSLClient.h that should only be included in .cpp files.
 */

#pragma once

#include <list>
#include <mutex>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <grpcpp/grpcpp.h>

#include "PTSL.grpc.pb.h"

#include "CppPTSLClient.h"
#include "PTSL_Versions.h"

namespace PTSLC_CPP
{
    /**
    * Common utility structure that contains grpc data
    * which are transferring between the grpc async calls.
    */
    class CallData
    {
    public:
        CallData() = default;
        ptsl::Request m_grpcRequest;
        ptsl::Response m_grpcResponse;
        grpc::ClientContext m_context;
        grpc::Status m_grpcStatus;
    };

    static google::protobuf::util::JsonOptions DefaultJsonWriteOptions()
    {
        google::protobuf::util::JsonOptions jOpts;
        jOpts.add_whitespace = true;
#if GOOGLE_PROTOBUF_VERSION < 5026000
        jOpts.always_print_primitive_fields = true;
#else
        jOpts.always_print_fields_with_no_presence = true;
#endif
        jOpts.preserve_proto_field_names = true;
        return jOpts;
    }

    static google::protobuf::util::JsonParseOptions DefaultJsonParseOptions()
    {
        google::protobuf::util::JsonParseOptions jOpts;
        jOpts.ignore_unknown_fields = true;
        return jOpts;
    }

    /**
     * PTSL specific exception class.
     * Used only internally in the PTSLC_CPP::CppPTSLClient.
     */
    class PTSLException : public std::runtime_error
    {
    public:
        /**
         * PTSLException constructor with parameter. Used for initialization of PTSLException.
         */
        explicit PTSLException(const std::string& message) : std::runtime_error(message)
        {
        }
    };

    /**
     * CppPTSLClient data which can't be used in public headers.
     */
    struct CppPTSLClient::InternalData
    {
        struct RpcContext
        {
            grpc::ClientContext m_grpcContext;
        };

        /// The ptsl::PTSL::Stub aka m_client, our view of the server's exposed services.
        std::unique_ptr<ptsl::PTSL::Stub> m_client;

        /// The producer-consumer queue we use to communicate asynchronously with the gRPC runtime.
        grpc::CompletionQueue m_completionQueue;

        /// map a request ID to the response message
        std::map<std::string, ptsl::Response> m_responsePool;

        /// Active RPC calls
        std::list<std::shared_ptr<RpcContext>> m_rpcContexts;
        std::mutex m_rpcContextsMutex;

#if defined(_WIN32)
        HMODULE m_winHandle = nullptr;
#elif defined(__APPLE__)
        void* m_macHandle = nullptr;
#endif
    };
} // namespace PTSLC_CPP
