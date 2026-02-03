// Copyright 2021, 2023 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation file for the CppAsync.h
 */

#include "CppAsync.h"

namespace PTSLC_CPP
{
    std::mutex SafeLogger::mLocker;

    void SafeLogger::SyncPrint(const std::string& iMsg)
    {
        std::lock_guard<std::mutex> lock(mLocker);
        std::cout << iMsg;
    }
} // namespace PTSLC_CPP
