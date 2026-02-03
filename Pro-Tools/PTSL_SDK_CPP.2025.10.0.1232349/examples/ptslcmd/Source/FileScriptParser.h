// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Declaration of the ptslcmd file script parser.
 */

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "PtslCmdCommandRequest.h"

/**
 * Structured file script data.
 */
struct PtslCmdFileScript
{
    std::vector<PtslCmdCommandRequest> CommandRequests;
};

/**
 * Class for parsing file script. See Parse().
 */
class FileScriptParser
{
public:
    /**
     * Parses incoming text in the format of ptslcmd's file script returning structured data.
     */
    static std::optional<PtslCmdFileScript> Parse(const std::string& text);
};
