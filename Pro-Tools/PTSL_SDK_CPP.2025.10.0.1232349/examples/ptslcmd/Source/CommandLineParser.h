// Copyright 2022-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

using ParamsArgsMap = std::map<std::string, std::vector<std::string>>;

/**
 * Parses command line arguments into internal structure of parameters and arguments.
 *
 * Following terms are used in the context of this class:
 * - 'parameter' ('param') is a value that starts with a paramSymbol (e.g., "-") – e.g., "-file";
 * - 'argument' ('arg') is a value that follows a parameter – e.g., "script.txt".
 * Parameters can have single, multiple or no arguments.
 */
class CommandLineParser
{
public:
    CommandLineParser() = delete;
    CommandLineParser(int argc, char* argv[], bool skipFirstParam = true);
    CommandLineParser(std::vector<std::string> commandArguments, bool skipFirstParam = true);

    /**
     * Returns the map of parameters with their arguments.
     * The special key Constants::PARAMETERLESS_ARGS_KEY is used for arguments without a parameter.
     */
    const ParamsArgsMap& GetParamsWithArgs() const
    {
        return mParamsWithArgs;
    }

    /**
     * Checks if the command line arguments are empty.
     */
    bool IsEmpty() const;

    /**
     * Checks if the given parameter is present in the command line arguments.
     */
    bool HasParam(const std::string& param) const;

    /**
     * Returns all arguments for the given parameter.
     * If the parameter is a modifier (i.e., it does not have arguments), returns an empty vector.
     * If the parameter is not present, returns nullopt.
     */
    std::optional<std::vector<std::string>> GetArgsForParam(const std::string& param) const;

    /**
     * Returns the first argument for the given parameter.
     * If the parameter is not present or has no arguments, returns the default value.
     */
    std::string FirstOrDefault(const std::string& param, const std::string& defaultValue) const;

    // FirstOrDefault with a default value of "" syntax sugar:
    std::string operator[](const std::string& param) const;

private:
    void ParseParamsWithArgs(const std::vector<std::string>& cmdItems, bool skipFirst = true);

    ParamsArgsMap mParamsWithArgs;
};
