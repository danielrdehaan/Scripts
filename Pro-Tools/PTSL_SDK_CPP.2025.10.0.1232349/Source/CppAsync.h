// Copyright 2020-2021, 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Utilities for asynchronous operations.
 */

#pragma once

#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

namespace PTSLC_CPP
{
    /**
     * Class that describes specific TaskCanceledException based on runtime_error using to communicate task cancellation. 
     * Throws when AsyncDelay reached a time out. Equivalent of C#'s TaskCanceledException class.
     */
    class TaskCancelledException : public std::runtime_error
    {
    public:
        /**
         * TaskCancelledException constructor. Used for initialization of TaskCancelledException.
         */
        TaskCancelledException() : std::runtime_error("TaskCancelledException")
        {
        }
    };

    /**
     * Utility method that checks whether cancellation of Task (std::future<void>) is required. 
     * Work with async tasks implemented by analogy with C#.
     */
    template <class T>
    bool IsCancellationRequested(const std::future<T>& cancellation)
    {
        bool ready = cancellation.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
        return ready;
    }

    /**
     * Utility method that asynchronously waits for the specified task to complete within a specified delay. 
     * The method throws a TaskCancelledException when the wait completes. Implemented by analogy with C#'s Task.Delay.
     */
    template <class T>
    void AsyncDelay(const std::future<T>& cancellation, std::chrono::milliseconds delay = std::chrono::milliseconds(1))
    {
        if (cancellation.wait_for(delay) == std::future_status::ready)
        {
            throw TaskCancelledException();
        }
    }

    /**
     * Utility method that runs the function func asynchronously (potentially in a separate thread which might be a part of 
     * a thread pool) and waits for the result of this function, and returns the result. Implemented by analogy with C#'s async/await.
     */
    template <class Request, class Response, class ClientPtr, class ClientFunctionPtr>
    std::shared_ptr<Response> AsyncWait(ClientFunctionPtr func, ClientPtr ptr, const Request& request)
    {
        std::future<std::shared_ptr<Response>> future = std::async(std::launch::async, func, ptr, request);
        std::shared_ptr<Response> response = future.get();
        return response;
    }

    template <class ClientPtr, class ClientFunction, class... Args>
    void AsyncWait2(ClientPtr caller, ClientFunction callerFunc, Args&&... args)
    {
        std::future<void> future = std::async(std::launch::async, callerFunc, caller, std::forward<Args>(args)...);
        future.get();
    }

    /**
    * Utility class for a thread-safe logging and printing error or warning messages.
    */
    class SafeLogger
    {
    public:
        static void SyncPrint(const std::string& iMsg);

    private:
        static std::mutex mLocker;
    };
} // namespace PTSLC_CPP
