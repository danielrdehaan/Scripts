// Copyright 2020-2021, 2023-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Encryption utilities.
 */

#pragma once

#include "PtslCCppExport.h"

#include <string>
#include <vector>

namespace PTSLC_CPP
{
    /**
     * Encrypts a text string with given RSA key.
     */
    std::vector<unsigned char> PTSLC_CPP_EXPORT encrypt(const std::string& text, const std::string& encryptionKey);
    
    /**
     * Encodes byte array into base 64 string.
     */
    std::string PTSLC_CPP_EXPORT encodeBase64(const std::vector<unsigned char>& data);
} // namespace PTSLC_CPP
