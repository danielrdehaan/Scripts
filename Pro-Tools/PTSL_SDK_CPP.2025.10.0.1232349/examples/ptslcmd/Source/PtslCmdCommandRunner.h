// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PTSLC_CPP/CppPTSLClient.h"
#include "PtslCmdCommandRequest.h"
#include "PtslCmdCommandResult.h"

class PtslCmdCommandRunner
{
public:
    explicit PtslCmdCommandRunner(PTSLC_CPP::CppPTSLClient& client) : mClientPtr(&client)
    {
    }

    virtual ~PtslCmdCommandRunner() = default;

    PtslCmdCommandStreamResult RunCommand(const PtslCmdCommandRequest& ptslCmdRequest);
    std::vector<PtslCmdCommandStreamResult> RunScript(const std::string& scriptJson);
    std::vector<PtslCmdCommandStreamResult> RunFileScript(const std::string& fileName);

private:
    PtslCmdCommandRunner() = default;

    PTSLC_CPP::CppPTSLClient* mClientPtr;
};
