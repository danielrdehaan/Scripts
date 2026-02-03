// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#include <iostream>
#include <optional>

#include "CommandLineParser.h"
#include "Constants.h"

using namespace std;

CommandLineParser::CommandLineParser(int argc, char* argv[], bool skipFirstParam)
    : CommandLineParser(vector<string>(argv, argv + argc), skipFirstParam)
{
}

CommandLineParser::CommandLineParser(vector<string> commandArguments, bool skipFirstParam)
{
    ParseParamsWithArgs(commandArguments, skipFirstParam);
}

bool CommandLineParser::IsEmpty() const
{
    return mParamsWithArgs.empty();
}

bool CommandLineParser::HasParam(const string& param) const
{
    return mParamsWithArgs.find(param) != mParamsWithArgs.end();
}

optional<vector<string>> CommandLineParser::GetArgsForParam(const string& param) const
{
    if (!HasParam(param))
    {
        return nullopt;
    }

    return mParamsWithArgs.find(param)->second;
}

string CommandLineParser::FirstOrDefault(const string& param, const string& defaultValue) const
{
    auto args = GetArgsForParam(param);
    return args && !args->empty() ? args->front() : defaultValue;
}

string CommandLineParser::operator[](const string& param) const
{
    return FirstOrDefault(param, "");
}

void CommandLineParser::ParseParamsWithArgs(const std::vector<string>& cmdItems, bool skipFirst)
{
    string currParam = Constants::PARAMETERLESS_ARGS_KEY;

    size_t start = skipFirst ? 1 : 0;
    for (size_t i = start; i < cmdItems.size(); ++i)
    {
        const std::string item = cmdItems.at(i);

        if (!item.empty() && item.at(0) == Constants::PARAMETER_PREFIX)
        {
            currParam = item;
            mParamsWithArgs[currParam] = {};
            continue;
        }

        mParamsWithArgs[currParam].push_back(item);
    }
}
