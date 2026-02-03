// Copyright 2020-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief C++ PTSL client test application.
 *
 * This application is used internally in Avid to test the PTSL Scripting SDK.
 * You can use the application at your own risk to send Pro Tools commands without creating your own program.
 * However, we can't guarantee that it won't be changed or removed in the future.
 *
 * Please see usage information by running:
 * \code{.sh}
 * ptslcmd -help
 * \endcode
 */

#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "CommandLineParser.h"
#include "Constants.h"
#include "PrintHelpHelpers.h"
#include "PtslCmdCommandRequest.h"
#include "PtslCmdCommandResult.h"
#include "PtslCmdCommandRunner.h"

#include "PTSLC_CPP/CppPTSLClient.h"
#include "PTSLC_CPP/CppPTSLCommonConversions.h"

#define NO_RESULT 0
#define RESET "\033[0m"
#define ORANGE "\033[38;5;214m"
#define BLUE "\033[38;5;33m"

using namespace std;
using namespace PTSLC_CPP;

using TaskQueue = vector<shared_ptr<CppPTSLResponse>>;

static TaskQueue s_CompletionTasksQueue;

static void PrintPtslCommandResults(const vector<PtslCmdCommandStreamResult>& cmdStreamResults);
static bool PrintHelpIfRequired(bool hasHelpParam, bool hasListParam, bool hasNoArgs);

int main(int argc, char* argv[])
{
    const auto params = CommandLineParser(argc, argv);

    if (PrintHelpIfRequired(
            params.HasParam(Constants::HELP_CMD_ARG), params.HasParam(Constants::LIST_CMD_ARG), params.IsEmpty()))
    {
        return NO_RESULT; // usage help or command list was printed
    }

    CppPTSLClient client({ "localhost:31416", Mode::ProTools, SkipHostLaunch::No });
    client.OnResponseReceived = [](shared_ptr<CppPTSLResponse> response)
    {
        cout << "Response received for task " << response->GetTaskId() << endl;
        s_CompletionTasksQueue.push_back(response);
    };

    const auto sessionId = params[Constants::SESSION_ID_CMD_ARG];
    if (!sessionId.empty())
    {
        client.SetSessionId(sessionId);
    }

    PtslCmdCommandRunner runner(client);
    vector<PtslCmdCommandStreamResult> cmdResults;
    if (params.HasParam(Constants::FILE_CMD_ARG))
    {
        const string fileName = params[Constants::FILE_CMD_ARG];
        cmdResults = runner.RunFileScript(fileName);
    }
    else
    {
        PtslCmdCommandRequest ptslCmdRequest;
        // First parameterless argument in `params` must be the API command name:
        ptslCmdRequest = { params[Constants::PARAMETERLESS_ARGS_KEY], {0, 0, 0}, params[Constants::JSON_REQUEST_CMD_ARG], params[Constants::JSON_HEADER_CMD_ARG], params[Constants::ENCRYPTION_KEY_ARG] };
        cmdResults = { runner.RunCommand(ptslCmdRequest) };
    }

    PrintPtslCommandResults(cmdResults);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

void PrintPtslCommandResults(const vector<PtslCmdCommandStreamResult>& cmdStreamResults)
{
    for (const auto& cmdResults : cmdStreamResults)
    {
        for (const auto& cmdResult : cmdResults.results)
        {
            if (!cmdResult.gotResponse)
            {
                if (!cmdResult.commandName.empty())
                {
                    cerr << endl << "Couldn't get response for " << cmdResult.commandName << ". ";
                }

                if (cmdResult.cannotParseArgsReason)
                {
                    cerr << *cmdResult.cannotParseArgsReason;
                }
                cerr << endl << endl;

                return;
            }

            const string cmdResStr = cmdResult.ToString();
            if (!cmdResStr.empty())
            {
                // '-'x50, it is used in Testing Framework, so please don't change it:
                const string divider = "--------------------------------------------------";

                cout << endl
                    << cmdResult.commandName << " JSON result:" << endl
                    << divider << endl
                    << cmdResStr << divider << endl
                    << endl;
            }
        }
    }
}

bool PrintHelpIfRequired(bool hasHelpParam, bool hasListParam, bool hasNoArgs)
{
    bool isPrinted = false;

    if (hasHelpParam || hasNoArgs)
    {
        PrintHelpHelpers::PrintHelp();
        isPrinted = true;
    }

    if (hasListParam || hasNoArgs)
    {
        PrintHelpHelpers::PrintCommandListHelp();
        isPrinted = true;
    }

    return isPrinted;
}

// Tests command execution and provides colorized output for the results saved in s_CompletionTasksQueue:
TEST(Client, Result)
{
    if (s_CompletionTasksQueue.empty())
    {
        cerr << "No response received (completion subCompletionTasksQueue queue is empty)." << endl;
        return;
    }

    // Group completion queue by TaskId:
    unordered_map<string, TaskQueue> groupedCompletionTasksQueue;
    for (const auto& task : s_CompletionTasksQueue)
    {
        groupedCompletionTasksQueue[task->GetTaskId()].push_back(task);
    }

    for (const auto& group : groupedCompletionTasksQueue)
    {
        const TaskQueue& subCompletionTasksQueue = group.second;

        // Iterate over subCompletionTasksQueue of each TaskId group:
        for (size_t idx = 0; idx < subCompletionTasksQueue.size(); ++idx)
        {
            const CommandStatusType status = subCompletionTasksQueue[idx]->GetStatus();
            const string taskId = subCompletionTasksQueue[idx]->GetTaskId();
            const auto commandType = subCompletionTasksQueue[idx]->GetCommandId();
            const auto commandName = EnumToString(commandType);

            // Print status (if failed) ----------------------------------
            map<CommandStatusType, string> statusMessages = {
                { CommandStatusType::Failed, " failed." },
                { CommandStatusType::FailedWithBadErrorResponse, " failed with a bad error response." },
                { CommandStatusType::CompletedWithBadResponse, " completed with a bad response." },
                { CommandStatusType::NoResponseReceived, " did not return any response." },
            };
            if (statusMessages.find(status) != statusMessages.end())
            {
                cerr << endl << "Command " << commandName << statusMessages[status] << endl;
            }

            // Print errors/warnings -------------------------------------
            const auto responseError = subCompletionTasksQueue[idx]->GetResponseErrorList();
            for (const auto& error : responseError.errors)
            {
                auto errTypeNo = static_cast<int>(error->errorType);
                string errTypeStr = EnumToString(error->errorType);

                auto errStr = error->errorMessage;
                bool isWarning = error->isWarning;

                if (isWarning)
                {
                    const int infoWarning = 200;
                    if (errTypeNo == infoWarning)
                    {
                        cerr << BLUE << "Info ";
                    }
                    else
                    {
                        cerr << ORANGE << "Warning ";
                    }
                }
                else
                {
                    cerr << "Error ";
                }

                cerr << errTypeNo << ": "
                     << "(" << errTypeStr << ") " << errStr << (isWarning ? RESET : "") << endl;
            }

            // Do assertions --------------------------------------------
            if (commandType == CommandId::HostReadyCheck || commandType == CommandId::GetTaskStatus)
            {
                EXPECT_EQ(status, CommandStatusType::Completed);
            }
            else
            {
                EXPECT_NE(status, CommandStatusType::Failed);
            }
        }
    }
}
