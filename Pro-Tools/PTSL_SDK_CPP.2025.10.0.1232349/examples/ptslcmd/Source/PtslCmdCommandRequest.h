// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

#include <optional>
#include <string>

/**
 * Structured PTSL Cmd abstraction fot command request data.
 */
struct PtslCmdCommandRequest
{
    std::string CommandName;
    std::tuple<int, int, int> Version {0, 0, 0};
    std::string RequestBodyJson = "";
    std::string VersionedHeader = "";
    std::string EncryptionKey = "";
};
