// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#include "CommandLineParser.h"

#include <cctype>

#define COMMAND_USAGE(CMD)                                                                                             \
    {                                                                                                                  \
        g_psz##CMD, g_psz##CMD##Help                                                                                   \
    }

const std::string CommandLineParser::PARAMETERLESS_ARGS_KEY = "_";
const std::string CommandLineParser::PARAMETER_MISSED = "Missed required parameter(s):";
const std::string CommandLineParser::PARAMETER_WRONG_USAGE = "Wrong usage of parameter(s):";
const std::string CommandLineParser::PARAMETERLESS_ARGS =
    "Unknown parameter. Please check if you missed '-' before:";
const std::string CommandLineParser::INVALID_COMMAND = "Invalid command.";
const std::string CommandLineParser::NO_RESPONSE_RECEIVED = "No response received.";

static const std::string PTSLCMD_USAGE = "ptslcmd usage:";
static const std::string COMMAND_LIST = "Command list:";

CommandLineParser::PtslcmdModifiers CommandLineParser::RetrievePtslcmdModifiers(
    const std::vector<std::string>& cmdArgs, bool containsExePath)
{
    const auto cmdStartOffset = (containsExePath ? 1 : 0);
    const auto cmdStart = cmdArgs.begin() + cmdStartOffset;

    // TODO: (ashevche, 24.01.2024) We need a separate common header file containing these constants:
    const auto hasHelp = std::find(cmdStart, cmdArgs.end(), "-help") != cmdArgs.end();
    const auto hasList = std::find(cmdStart, cmdArgs.end(), "-list") != cmdArgs.end();
    const auto hasFile = std::find(cmdStart, cmdArgs.end(), "-file") != cmdArgs.end();
    const auto hasJsonRequest = std::find(cmdStart, cmdArgs.end(), "-json_request") != cmdArgs.end();
    const auto hasJsonOutput = std::find(cmdStart, cmdArgs.end(), "-json_output") != cmdArgs.end();

    PtslcmdModifiers modifiers;
    modifiers.isHelp = hasHelp || hasList || cmdArgs.size() == cmdStartOffset;
    modifiers.isNamesListOnly = hasList;
    modifiers.useFileScript = hasFile;
    modifiers.useJsonRequest = hasJsonRequest;
    modifiers.useJsonOutput = hasJsonOutput;

    return modifiers;
}

bool CommandLineParser::HasPtslCommand(const std::vector<std::string>& cmdArgs, size_t ptslCommandPos)
{
    return cmdArgs.size() > ptslCommandPos
        && CommandLineParser::GetCmdUsages().count(cmdArgs.at(ptslCommandPos)) > 0;
}

std::map<std::string, std::vector<std::string>> CommandLineParser::RetrieveParamsWithArgs(
    const std::vector<std::string>& params, const char paramSymbol)
{
    std::map<std::string, std::vector<std::string>> paramsWithArgs;
    std::string currParam = PARAMETERLESS_ARGS_KEY;

    for (size_t i = 0; i < params.size(); ++i)
    {
        if (params.at(i).at(0) == paramSymbol)
        {
            currParam = params.at(i).substr(1); // without '-' at the beginning
            paramsWithArgs[currParam] = {};
            continue;
        }

        paramsWithArgs[currParam].push_back(params.at(i));
    }

    return paramsWithArgs;
}

bool CommandLineParser::IsNumber(const std::string& str)
{
    return !str.empty()
        && std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
}

bool CommandLineParser::StringToBool(const std::string& str)
{
    std::string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr == "true" || lowerStr == "t" || lowerStr == "yes" || lowerStr == "y" || lowerStr == "1";
}

void CommandLineParser::PrintListCommands(bool namesOnly, bool toFormat)
{
    std::cout << (namesOnly ? COMMAND_LIST : PTSLCMD_USAGE) << std::endl;

    for (const auto& cmdUsage : CommandLineParser::GetCmdUsages())
    {
        if (!namesOnly)
        {
            std::cout << std::endl;
        }

        CommandLineParser::DoPrintCommandHelp(cmdUsage.first, toFormat, namesOnly);
    }
}

void CommandLineParser::PrintHelp(const std::string& command, bool toFormat)
{
    std::cout << PTSLCMD_USAGE << std::endl;

    CommandLineParser::DoPrintCommandHelp(command, toFormat);
}

const std::map<std::string, std::string>& CommandLineParser::GetCmdUsages()
{
    static const std::map<std::string, std::string> COMMAND_USAGES = {
        COMMAND_USAGE(CreateSession),
        COMMAND_USAGE(OpenSession),
        COMMAND_USAGE(SaveSession),
        COMMAND_USAGE(SaveSessionAs),
        COMMAND_USAGE(CloseSession),
        COMMAND_USAGE(Import),
        COMMAND_USAGE(GetTrackList),
        COMMAND_USAGE(SelectAllClipsOnTrack),
        COMMAND_USAGE(ExtendSelectionToTargetTracks),
        COMMAND_USAGE(TrimToSelection),
        COMMAND_USAGE(CreateFadesBasedOnPreset),
        COMMAND_USAGE(RenameTargetTrack),
        COMMAND_USAGE(ConsolidateClip),
        COMMAND_USAGE(ExportClipsAsFiles),
        COMMAND_USAGE(ExportSelectedTracksAsAAFOMF),
        COMMAND_USAGE(GetTaskStatus),
        COMMAND_USAGE(RefreshTargetAudioFiles),
        COMMAND_USAGE(RefreshAllModifiedAudioFiles),
        COMMAND_USAGE(GetFileLocation),
        COMMAND_USAGE(Cut),
        COMMAND_USAGE(Copy),
        COMMAND_USAGE(Paste),
        COMMAND_USAGE(Clear),
        COMMAND_USAGE(CutSpecial),
        COMMAND_USAGE(CopySpecial),
        COMMAND_USAGE(ClearSpecial),
        COMMAND_USAGE(PasteSpecial),
        COMMAND_USAGE(ExportMix),
        COMMAND_USAGE(Spot),
        COMMAND_USAGE(ExportSessionInfoAsText),
        COMMAND_USAGE(SetPlaybackMode),
        COMMAND_USAGE(SetRecordMode),
        COMMAND_USAGE(GetDynamicProperties),
        COMMAND_USAGE(GetSessionAudioFormat),
        COMMAND_USAGE(GetSessionSampleRate),
        COMMAND_USAGE(GetSessionBitDepth),
        COMMAND_USAGE(GetSessionInterleavedState),
        COMMAND_USAGE(GetSessionTimeCodeRate),
        COMMAND_USAGE(GetSessionFeetFramesRate),
        COMMAND_USAGE(GetSessionAudioRatePullSettings),
        COMMAND_USAGE(GetSessionVideoRatePullSettings),
        COMMAND_USAGE(GetSessionName),
        COMMAND_USAGE(GetSessionPath),
        COMMAND_USAGE(GetSessionStartTime),
        COMMAND_USAGE(GetSessionLength),
        COMMAND_USAGE(SetSessionAudioFormat),
        COMMAND_USAGE(SetSessionBitDepth),
        COMMAND_USAGE(SetSessionInterleavedState),
        COMMAND_USAGE(SetSessionTimeCodeRate),
        COMMAND_USAGE(SetSessionFeetFramesRate),
        COMMAND_USAGE(SetSessionAudioRatePullSettings),
        COMMAND_USAGE(SetSessionVideoRatePullSettings),
        COMMAND_USAGE(SetSessionStartTime),
        COMMAND_USAGE(SetSessionLength),
        COMMAND_USAGE(GetPTSLVersion),
        COMMAND_USAGE(GetPlaybackMode),
        COMMAND_USAGE(GetRecordMode),
        COMMAND_USAGE(GetTransportArmed),
        COMMAND_USAGE(GetTransportState),
        COMMAND_USAGE(ClearMemoryLocation),
        COMMAND_USAGE(RenameSelectedClip),
        COMMAND_USAGE(RenameTargetClip),
        COMMAND_USAGE(TogglePlayState),
        COMMAND_USAGE(ToggleRecordEnable),
        COMMAND_USAGE(PlayHalfSpeed),
        COMMAND_USAGE(RecordHalfSpeed),
        COMMAND_USAGE(EditMemoryLocation),
        COMMAND_USAGE(CreateMemoryLocation),
        COMMAND_USAGE(GetMemoryLocations),
        COMMAND_USAGE(RegisterConnection),
        COMMAND_USAGE(CreateNewTracks),
        COMMAND_USAGE(GetEditMode),
        COMMAND_USAGE(SetEditMode),
        COMMAND_USAGE(GetEditTool),
        COMMAND_USAGE(SetEditTool),
        COMMAND_USAGE(RecallZoomPreset),
        COMMAND_USAGE(GetEditModeOptions),
        COMMAND_USAGE(SetEditModeOptions),
        COMMAND_USAGE(SetTimelineSelection),
        COMMAND_USAGE(GetTimelineSelection),
        COMMAND_USAGE(SelectTracksByName),
        COMMAND_USAGE(ImportVideo),
        COMMAND_USAGE(SelectMemoryLocation),
        COMMAND_USAGE(SetTrackMuteState),
        COMMAND_USAGE(SetTrackSoloState),
        COMMAND_USAGE(SetTrackSoloSafeState),
        COMMAND_USAGE(SetTrackRecordEnableState),
        COMMAND_USAGE(SetTrackRecordSafeEnableState),
        COMMAND_USAGE(SetTrackInputMonitorState),
        COMMAND_USAGE(SetTrackSmartDspState),
        COMMAND_USAGE(SetTrackHiddenState),
        COMMAND_USAGE(SetTrackInactiveState),
        COMMAND_USAGE(SetTrackFrozenState),
        COMMAND_USAGE(SetTrackOnlineState),
        COMMAND_USAGE(SetTrackOpenState),
        COMMAND_USAGE(GetSessionIDs),
        COMMAND_USAGE(GetMemoryLocationsManageMode),
        COMMAND_USAGE(SetMemoryLocationsManageMode),
        COMMAND_USAGE(SetMainCounterFormat),
        COMMAND_USAGE(SetSubCounterFormat),
        COMMAND_USAGE(GetMainCounterFormat),
        COMMAND_USAGE(GetSubCounterFormat),
        COMMAND_USAGE(Undo),
        COMMAND_USAGE(Redo),
        COMMAND_USAGE(UndoAll),
        COMMAND_USAGE(RedoAll),
        COMMAND_USAGE(ClearUndoQueue),
        COMMAND_USAGE(SetTrackDSPModeSafeState),
        COMMAND_USAGE(GetSessionSystemDelayInfo),
        COMMAND_USAGE(GroupClips),
        COMMAND_USAGE(UngroupClips),
        COMMAND_USAGE(UngroupAllClips),
        COMMAND_USAGE(RegroupClips),
        COMMAND_USAGE(HostReadyCheck),
        COMMAND_USAGE(RepeatSelection),
        COMMAND_USAGE(DuplicateSelection),
        COMMAND_USAGE(ClearAllMemoryLocations),
    };

    return COMMAND_USAGES;
}

void CommandLineParser::DoPrintFormattedUsage(const std::string& usage)
{
    if (usage.empty())
    {
        return;
    }

    const char tab = '\t';
    const char spc = ' ';
    int32_t sqrBracketCnt = 0, triagBracketCnt = 0;
    bool needPrintSpace = false;
    char prev = '^';

    std::cout << "  ";

    for (char curr : usage)
    {
        if (needPrintSpace)
        {
            std::cout << spc;
            needPrintSpace = false;

            if (curr == spc)
            {
                continue;
            }
        }

        switch (curr)
        {
        case '[':
        {
            if (sqrBracketCnt < 1 && triagBracketCnt < 1 && prev != '-')
            {
                std::cout << std::endl << tab;
            }
            ++sqrBracketCnt;
            break;
        }
        case ']':
        {
            --sqrBracketCnt;
            break;
        }
        case '<':
        {
            ++triagBracketCnt;
            break;
        }
        case '>':
        {
            --triagBracketCnt;
            break;
        }
        case '{':
        {
            needPrintSpace = true;
            break;
        }
        case '}':
        {
            if (prev != spc)
            {
                std::cout << spc;
            }
            break;
        }
        case '-':
        {
            if (sqrBracketCnt < 1 && triagBracketCnt < 1)
            {
                std::cout << std::endl << tab;
            }
            break;
        }
        case '|':
        {
            if (prev != spc)
            {
                std::cout << spc;
            }
            needPrintSpace = true;
            break;
        }
        }

        if (curr != '\n')
        {
            std::cout << curr;
        }

        prev = curr;
    }

    std::cout << std::endl;
}

void CommandLineParser::DoPrintCommandHelp(const std::string& command, bool toFormat, bool nameOnly)
{
    if (CommandLineParser::GetCmdUsages().count(command) == 0)
    {
        std::cout << INVALID_COMMAND << std::endl;
        return;
    }

    const std::string usage = (!nameOnly && !CommandLineParser::GetCmdUsages().at(command).empty())
        ? CommandLineParser::GetCmdUsages().at(command)
        : command;

    if (toFormat)
    {
        CommandLineParser::DoPrintFormattedUsage(usage);
    }
    else
    {
        std::cout << usage << std::endl;
    }
}
