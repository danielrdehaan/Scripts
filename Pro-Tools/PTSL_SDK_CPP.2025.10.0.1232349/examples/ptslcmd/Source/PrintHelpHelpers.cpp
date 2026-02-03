// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#include <algorithm>
#include <iostream>

#include "PrintHelpHelpers.h"
#include "PTSLC_CPP/CppPTSLCommonConversions.h"

using namespace std;
using namespace PTSLC_CPP;

void PrintHelpHelpers::PrintCommandListHelp()
{
    cout << "COMMAND LIST:" << endl;
    cout << "--" << endl;

    auto enumValueStrings = EnumValuesToStrings("CommandId");
    std::sort(enumValueStrings.begin(), enumValueStrings.end());

    for (const auto& enumValue : enumValueStrings)
    {
        cout << enumValue << endl;
    }

    cout << endl;
}

void PrintHelpHelpers::PrintHelp()
{
    cout << "PTSL CMD USAGE:" << endl;
    cout << "--";
    cout << COMMON_USAGE_HELP << endl;
    cout << endl;
}

// Please remember to update this help string when making changes to the PTSL Cmd:
const std::string PrintHelpHelpers::COMMON_USAGE_HELP = R"(
To view general PTSL Cmd usage information:
    ptslcmd -help

To print all the PTSL commands list:
    ptslcmd -list

To run a PTSL command with an optionally passed request in a JSON form:
    ptslcmd <command> [-json_request "{ your escaped JSON request goes here }"] [-json_header "{ your escaped JSON header goes here }"] [-header:session_id <session_id>] [-encryption_key <encryption_key>]

    e.g., ptslcmd GetSessionName -header:session_id "0f584056-0f36-4f36-ac4c-af261c46142e"

    or    ptslcmd Undo -json_request "{\"levels\": 2}" -json_header "{ \"batch_job_header\": { \"id\": \"d71ec4bc-40ab-4d6a-afd3-ea33c31fd821\" } }" -header:session_id "0f584056-0f36-4f36-ac4c-af261c46142e"

To obtain the `session_id`, you can use the `RegisterConnection` API command. It will be valid until you close Pro Tools.
    e.g., ptslcmd RegisterConnection -json_request "{\"company_name\": \"YOUR_COMPANY\", \"application_name\": \"YOUR_APP\"}"

To run a JSON file script:
    ptslcmd -file <path-to-your-script-file> [-header:session_id <session_id>]

The example of a JSON file script (it can contain // or /* */ comment types):

{
  "commands": [
    /*
     * Please notice `RegisterConnection` command is required
     * to be the first command in the script
     * if you don't provide `session_id` in the command line.
     */
    {
      "command_name": "RegisterConnection",
      "json_request": {
        "company_name": "YOUR_COMPANY",
        "application_name": "YOUR_APP"
      }
    },
    {
      "command_name": "OpenSession",
      "json_request": {
        "session_path": "/path-to-session/your-session.ptx"
      }
    },
    {
      "command_name": "GetSessionName" // example of requestless command
    }
  ]
}

Please find more information about specific PTSL command requests and responses in the PTSL documentation.
)";
