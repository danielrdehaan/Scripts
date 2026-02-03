// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

#include <string>

/**
 * Helper class for printing help/usage information.
 */
class PrintHelpHelpers
{
public:
    /**
     * Prints PTSL Cmd usage.
     */
    static void PrintHelp();

    /**
     * Prints a list of PTSL API commands.
     */
    static void PrintCommandListHelp();

private:
    static const std::string COMMON_USAGE_HELP;
};
