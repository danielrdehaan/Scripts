// Copyright 2020-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief C++ PTSL client test application.
 * 
 * Use this application to send Pro Tools commands without creating your own program.
 * 
 * To send multiple commands create a file with each line being a separate command (or it can be a .json file where commands are structured in a JSON format) and call:
 * \code{.sh}
 * ptslcmd -file <filepath>
 * \endcode
 *
 * The file can contain such comment types:
 * - Python style: # Comment
 * - C++ style: // Comment
 * - C style (single-/multiline): / * Comment * / <- remove space between / and *
 *
 * To view all commands:
 * \code{.sh}
 * ptslcmd -help
 * \endcode
 *
 * Or to view specific command:
 * \code{.sh}
 * ptslcmd <command> -help
 * \endcode
 *
 * To print commands list:
 * \code{.sh}
 * ptslcmd -list
 * \endcode
 *
 * To pass the request in a JSON form:
 * \code{.sh}
 * ptslcmd <command> -json_request "{ your json goes here }"
 * \endcode
 *
 * Example usage of the -file option:
 *
 * Create a file - commands.txt.
 * \code
 * RegisterConnection -company_name "MY COMPANY NAME" -application_name "MY APPLICATION NAME"
 * GetSessionName
 * \endcode
 *
 * Call ptslcmd:
 * \code{.sh}
 * ptslcmd -file /path/to/commands.txt
 * \endcode
 *
* Example usage of the -file option with JSON file:
 *
 * Create a file - commands.json.
 * \code{.json}
 * {
 *   "commands": [
 *     {
 *       "command_name": "RegisterConnection",
 *       "json_request": {
 *         "company_name": "MY COMPANY NAME",
 *         "application_name": "MY APPLICATION NAME"
 *       }
 *     },
 *     {
 *       "command_name": "SetTrackMuteState",
 *       "json_request": {
 *         "track_names": [
 *           "Audio1",
 *           "Audio2"
 *         ],
 *         "enabled": true
 *       }
 *     }
 *   ]
 * }
 * \endcode
 *
 * Call ptslcmd:
 * \code{.sh}
 * ptslcmd -file /path/to/commands.json
 * \endcode
 *
 * For any details check the COMMAND_NAME.cpp file.
 */

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "Common.h"
#include "FileScriptParser.h"
#include "PtslCmdCommandResult.h"

#define NO_RESULT 0

using CommandFunc = std::function<PtslCmdCommandResult(const std::vector<std::string>&, CppPTSLClient&)>;

static std::vector<std::shared_ptr<CommandResponse>> s_CompletionTasksQueue;

static std::map<std::string, CommandFunc>& GetCmdFunctions()
{
    static std::map<std::string, CommandFunc> s_commandFunctions = {
        { g_pszCreateSession, CreateSession },
        { g_pszOpenSession, OpenSession },
        { g_pszImport, Import },
        { g_pszGetTrackList, GetTrackList },
        { g_pszSelectAllClipsOnTrack, SelectAllClipsOnTrack },
        { g_pszExtendSelectionToTargetTracks, ExtendSelectionToTargetTracks },
        { g_pszTrimToSelection, TrimToSelection },
        { g_pszCreateFadesBasedOnPreset, CreateFadesBasedOnPreset },
        { g_pszRenameTargetTrack, RenameTargetTrack },
        { g_pszConsolidateClip, ConsolidateClip },
        { g_pszExportClipsAsFiles, ExportClipsAsFiles },
        { g_pszExportSelectedTracksAsAAFOMF, ExportSelectedTracksAsAAFOMF },
        { g_pszGetTaskStatus, GetTaskStatus },
        { g_pszRefreshTargetAudioFiles, RefreshTargetAudioFiles },
        { g_pszRefreshAllModifiedAudioFiles, RefreshAllModifiedAudioFiles },
        { g_pszGetFileLocation, GetFileLocation },
        { g_pszCloseSession, CloseSession },
        { g_pszSaveSession, SaveSession },
        { g_pszSaveSessionAs, SaveSessionAs },
        { g_pszCut, Cut },
        { g_pszCopy, Copy },
        { g_pszPaste, Paste },
        { g_pszClear, Clear },
        { g_pszCutSpecial, CutSpecial },
        { g_pszCopySpecial, CopySpecial },
        { g_pszClearSpecial, ClearSpecial },
        { g_pszPasteSpecial, PasteSpecial },
        { g_pszExportMix, ExportMix },
        { g_pszSpot, Spot },
        { g_pszExportSessionInfoAsText, ExportSessionInfoAsText },
        { g_pszSetPlaybackMode, SetPlaybackMode },
        { g_pszSetRecordMode, SetRecordMode },
        { g_pszGetDynamicProperties, GetDynamicProperties },
        { g_pszGetSessionAudioFormat, GetSessionAudioFormat },
        { g_pszGetSessionSampleRate, GetSessionSampleRate },
        { g_pszGetSessionBitDepth, GetSessionBitDepth },
        { g_pszGetSessionInterleavedState, GetSessionInterleavedState },
        { g_pszGetSessionTimeCodeRate, GetSessionTimeCodeRate },
        { g_pszGetSessionFeetFramesRate, GetSessionFeetFramesRate },
        { g_pszGetSessionAudioRatePullSettings, GetSessionAudioRatePullSettings },
        { g_pszGetSessionVideoRatePullSettings, GetSessionVideoRatePullSettings },
        { g_pszGetSessionName, GetSessionName },
        { g_pszGetSessionPath, GetSessionPath },
        { g_pszGetSessionStartTime, GetSessionStartTime },
        { g_pszGetSessionLength, GetSessionLength },
        { g_pszSetSessionAudioFormat, SetSessionAudioFormat },
        { g_pszSetSessionBitDepth, SetSessionBitDepth },
        { g_pszSetSessionInterleavedState, SetSessionInterleavedState },
        { g_pszSetSessionTimeCodeRate, SetSessionTimeCodeRate },
        { g_pszSetSessionFeetFramesRate, SetSessionFeetFramesRate },
        { g_pszSetSessionAudioRatePullSettings, SetSessionAudioRatePullSettings },
        { g_pszSetSessionVideoRatePullSettings, SetSessionVideoRatePullSettings },
        { g_pszSetSessionStartTime, SetSessionStartTime },
        { g_pszSetSessionLength, SetSessionLength },
        { g_pszGetPTSLVersion, GetPTSLVersion },
        { g_pszGetPlaybackMode, GetPlaybackMode },
        { g_pszGetRecordMode, GetRecordMode },
        { g_pszGetTransportArmed, GetTransportArmed },
        { g_pszGetTransportState, GetTransportState },
        { g_pszClearMemoryLocation, ClearMemoryLocation },
        { g_pszRenameSelectedClip, RenameSelectedClip },
        { g_pszRenameTargetClip, RenameTargetClip },
        { g_pszTogglePlayState, TogglePlayState },
        { g_pszToggleRecordEnable, ToggleRecordEnable },
        { g_pszPlayHalfSpeed, PlayHalfSpeed },
        { g_pszRecordHalfSpeed, RecordHalfSpeed },
        { g_pszEditMemoryLocation, EditMemoryLocation },
        { g_pszCreateMemoryLocation, CreateMemoryLocation },
        { g_pszGetMemoryLocations, GetMemoryLocations },
        { g_pszRegisterConnection, RegisterConnection },
        { g_pszCreateNewTracks, CreateNewTracks },
        { g_pszRegisterConnection, RegisterConnection },
        { g_pszGetEditMode, GetEditMode },
        { g_pszSetEditMode, SetEditMode },
        { g_pszGetEditTool, GetEditTool },
        { g_pszSetEditTool, SetEditTool },
        { g_pszRecallZoomPreset, RecallZoomPreset },
        { g_pszGetEditModeOptions, GetEditModeOptions },
        { g_pszSetEditModeOptions, SetEditModeOptions },
        { g_pszSetTimelineSelection, SetTimelineSelection },
        { g_pszGetTimelineSelection, GetTimelineSelection },
        { g_pszSelectTracksByName, SelectTracksByName },
        { g_pszImportVideo, ImportVideo },
        { g_pszSelectMemoryLocation, SelectMemoryLocation },
        { g_pszSetTrackMuteState, SetTrackMuteState },
        { g_pszSetTrackSoloState, SetTrackSoloState },
        { g_pszSetTrackSoloSafeState, SetTrackSoloSafeState },
        { g_pszSetTrackRecordEnableState, SetTrackRecordEnableState },
        { g_pszSetTrackRecordSafeEnableState, SetTrackRecordSafeEnableState },
        { g_pszSetTrackInputMonitorState, SetTrackInputMonitorState },
        { g_pszSetTrackSmartDspState, SetTrackSmartDspState },
        { g_pszSetTrackHiddenState, SetTrackHiddenState },
        { g_pszSetTrackInactiveState, SetTrackInactiveState },
        { g_pszSetTrackFrozenState, SetTrackFrozenState },
        { g_pszSetTrackOnlineState, SetTrackOnlineState },
        { g_pszSetTrackOpenState, SetTrackOpenState },
        { g_pszGetSessionIDs, GetSessionIDs },
        { g_pszGetMemoryLocationsManageMode, GetMemoryLocationsManageMode },
        { g_pszSetMemoryLocationsManageMode, SetMemoryLocationsManageMode },
        { g_pszSetMainCounterFormat, SetMainCounterFormat },
        { g_pszSetSubCounterFormat, SetSubCounterFormat },
        { g_pszGetMainCounterFormat, GetMainCounterFormat },
        { g_pszGetSubCounterFormat, GetSubCounterFormat },
        { g_pszUndo, Undo },
        { g_pszRedo, Redo },
        { g_pszUndoAll, UndoAll },
        { g_pszRedoAll, RedoAll },
        { g_pszClearUndoQueue, ClearUndoQueue },
        { g_pszSetTrackDSPModeSafeState, SetTrackDSPModeSafeState },
        { g_pszGetSessionSystemDelayInfo, GetSessionSystemDelayInfo },
        { g_pszGroupClips, GroupClips },
        { g_pszUngroupClips, UngroupClips },
        { g_pszUngroupAllClips, UngroupAllClips },
        { g_pszRegroupClips, RegroupClips },
        { g_pszRepeatSelection, RepeatSelection },
        { g_pszDuplicateSelection, DuplicateSelection },
        { g_pszClearAllMemoryLocations, ClearAllMemoryLocations },
    };

    return s_commandFunctions;
}

static int RunPtslCommandAndTests(CppPTSLClient& client, const std::vector<std::string>& cmdArgs, bool showJsonOutput,
    size_t ptslCommandPos = 1, bool runTests = true);
static int RunPtslCommandAndTestsFromFileScript(
    CppPTSLClient& client, const std::vector<std::string>& cmdArgs, bool showJsonOutput);
static std::string ExtractFromCommandLine(
    std::vector<std::string>& cmdParams, const std::string& param, const std::string& defaultValue);

namespace PTSLC_CPP
{
    std::ostream& operator<<(std::ostream& out, const CommandStatusType& type);
} // namespace PTSLC_CPP

int main(int argc, char* argv[])
{
    std::vector<std::string> commandArguments(argv, argv + argc);

    const auto modifiers = CommandLineParser::RetrievePtslcmdModifiers(commandArguments);

    if (modifiers.useJsonOutput)
    {
        // HACK: (ashevche, 14.09.2023): finds and removes "-json_output" from commandArguments (the modifier is saved anyway):
        // TODO: (ashevche, 14.09.2023): rethink and refactor all this algo once we get rid of cmd staff
        commandArguments.erase(
            std::remove(commandArguments.begin(), commandArguments.end(), "-json_output"), commandArguments.end());
    }

    if (modifiers.isHelp)
    {
        CommandLineParser::HasPtslCommand(commandArguments)
            ? CommandLineParser::PrintHelp(commandArguments[1])
            : // commandArguments.at(0) == "ptslcmd"
            CommandLineParser::PrintListCommands(modifiers.isNamesListOnly);

        return NO_RESULT;
    }

    CppPTSLClient client({ "localhost:31416", Mode::ProTools, SkipHostLaunch::No });

    const auto sessionId = ExtractFromCommandLine(commandArguments, "-header:session_id", std::string {});
    if (!sessionId.empty())
    {
        client.SetSessionId(sessionId);
    }

    client.OnResponseReceived_V1_Deprecated = [](shared_ptr<CommandResponse> response)
    { s_CompletionTasksQueue.push_back(response); };

    return modifiers.useFileScript
        ? RunPtslCommandAndTestsFromFileScript(client, commandArguments, modifiers.useJsonOutput)
        : RunPtslCommandAndTests(client, commandArguments, modifiers.useJsonOutput);
}

static void InitGoogleTests(const std::vector<std::string>& cmdArgs)
{
    auto argc = static_cast<int>(cmdArgs.size()); // InitGoogleTest requires int, also int main(int argc...)
    vector<char*> argv;
    argv.reserve(argc);

    for (int i = 0; i < argc; ++i)
    {
        argv.push_back(const_cast<char*>(cmdArgs.at(i).c_str()));
    }

    testing::InitGoogleTest(&argc, argv.data());
}

int RunPtslCommandAndTests(CppPTSLClient& client, const std::vector<std::string>& cmdArgs, bool showJsonOutput,
    size_t ptslCommandPos, bool runTests)
{
    if (!CommandLineParser::HasPtslCommand(cmdArgs, ptslCommandPos))
    {
        std::cout << CommandLineParser::INVALID_COMMAND << std::endl;
        return NO_RESULT;
    }

    static const auto commandFunctions = GetCmdFunctions();
    const string commandName = cmdArgs.at(ptslCommandPos);
    const CommandFunc cmdFunc = commandFunctions.at(commandName);

    std::vector<std::string> cmdParams(cmdArgs);
    cmdParams.erase(cmdParams.begin(),
        cmdParams.begin() + ptslCommandPos + 1); // command parameters without "ptslcmd" or CommandName in the beginning

    const PtslCmdCommandResult cmdResult = cmdFunc(cmdParams, client);
    const std::string cmdResStr = cmdResult.ToString();

    if (showJsonOutput && !cmdResStr.empty())
    {
        const std::string devider = "--------------------------------------------------"; // '-'x50
        std::cout << commandName << " JSON result:" << std::endl
                  << devider << cmdResStr << devider << std::endl
                  << std::endl;
    }

    if (cmdResult && runTests)
    {
        InitGoogleTests(cmdArgs);
        return RUN_ALL_TESTS();
    }

    return NO_RESULT;
}

int RunPtslCommandAndTestsFromFileScript(
    CppPTSLClient& client, const std::vector<std::string>& cmdArgs, bool showJsonOutput)
{
    if (cmdArgs.size() < 3)
    {
        std::cout << "Invalid file." << std::endl;
        return NO_RESULT;
    }

    std::string fileName = cmdArgs.at(2);
    // The objective is to retrieve the file extension. If it is ".json", we will parse it as a JSON file;
    // otherwise, we will process it as a command script file, as previously.
    auto fileExt = std::filesystem::path(fileName).extension().string();
    bool isJsonFile = (fileExt == ".json");

    ifstream file(fileName);
    std::string text { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    file.close();

    const std::optional<FileScript> script = FileScriptParser::Parse(text, isJsonFile);

    if (!script)
    {
        std::cout << "Failed to parse the script" << std::endl;
        return NO_RESULT;
    }

    for (const auto& command : script->Commands)
    {
        auto params = command.Arguments;
        params.insert(params.begin(), command.Name);

        const auto ptslCommandPos = 0; // 0 because in a script, we omit "ptslcmd" before each ptsl command
        RunPtslCommandAndTests(client, params, showJsonOutput, ptslCommandPos, /*runTests*/ false);

        if (command.PostLaunchDelayMs > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(command.PostLaunchDelayMs));
        }
    }

    InitGoogleTests(cmdArgs);
    return RUN_ALL_TESTS();
}

TEST(Client, Result)
{
    for (size_t idx = 0; idx < s_CompletionTasksQueue.size(); ++idx)
    {
        const PTSLC_CPP::CommandStatusType status = s_CompletionTasksQueue[idx]->status.type;
        const string statusClarification = s_CompletionTasksQueue[idx]->status.typeClarification;
        const auto errors = s_CompletionTasksQueue[idx]->errors;
        const auto command = s_CompletionTasksQueue[idx]->header.commandType;

        switch (status)
        {
        case CommandStatusType::CompletedWithBadResponse:
            cerr << "Command completed with a bad response" << endl;
            break;
        case CommandStatusType::Failed:
            cerr << "Command failed." << endl;
            break;
        case CommandStatusType::FailedWithBadErrorResponse:
            cerr << "Command failed with a bad error response" << endl;
            break;
        }

        for (const auto& error : errors)
        {
            cerr << (error->isWarning ? "Warning " : "Error ") << static_cast<int>(error->errorType) << ": "
                 << error->errorMessage << endl;
        }

        if (!statusClarification.empty())
        {
            cout << "Clarification for developers >>> " << statusClarification << endl << endl;
        }

        if (command == CommandType::HostReadyCheck || command == CommandType::GetTaskStatus)
        {
            EXPECT_EQ(status, CommandStatusType::Completed);
        }
        else
        {
            if (idx == 0)
            {
                EXPECT_EQ(status, CommandStatusType::Queued);
            }
            else if (idx == s_CompletionTasksQueue.size() - 1)
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

std::string ExtractFromCommandLine(
    std::vector<std::string>& cmdParams, const std::string& param, const std::string& defaultValue)
{
    auto iter = std::find(cmdParams.begin(), cmdParams.end(), param);
    if (iter == cmdParams.end())
    {
        return defaultValue;
    }

    if ((iter + 1) >= cmdParams.end())
    {
        std::cout << "Value for " << param << " argument is missing" << std::endl;
        return std::string {};
    }

    const auto value = *(iter + 1);
    cmdParams.erase(iter, iter + 2);
    return value;
}

namespace PTSLC_CPP
{
    std::ostream& operator<<(std::ostream& out, const CommandStatusType& type)
    {
        return out << static_cast<std::underlying_type<CommandStatusType>::type>(type);
    }
} // namespace PTSLC_CPP
