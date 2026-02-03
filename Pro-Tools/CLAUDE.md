# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

Avid Pro Tools Scripting Library (PTSL) C++ SDK (version 2025.10.0). Enables external applications to control Pro Tools via gRPC-based IPC on localhost:31416.

## Build Commands

Build the C++ client library:
```sh
cd PTSL_SDK_CPP.2025.10.0.1232349
python3 setup/build_cpp_ptsl_sdk.py --target ptsl.client.cpp --config Debug
```

Build the ptslcmd example application:
```sh
python3 setup/build_cpp_ptsl_sdk.py --target ptslcmd.sdk --config Debug
```

Build options:
- `--config Debug|Release` - Build configuration
- `--arch x86_64|arm64|x86_64_arm64` - Target architecture (Mac)
- `--library_type shared|static` - Library type (default: shared)
- `--vs_compiler vs2017|vs2019|vs2022` - Visual Studio version (Windows)

Output is placed in the SDK's `install` directory.

## Prerequisites

- Python >= 3.8
- Mac: Xcode >= 10.2.1
- Windows: Visual Studio 2017/2019/2022, Windows SDK 11

The build script creates a venv, installs Conan 1.x dependencies, and runs CMake.

## Architecture

### PTSL Client Wrapper (`Source/`)
- `CppPTSLClient.h/.cpp` - Main client class managing gRPC connection to Pro Tools
- `CppPTSLRequest.h/.cpp` - Request object wrapping command ID and JSON body
- `CppPTSLResponse.h/.cpp` - Response object with status, body, and error handling
- `CppPTSLCommon.h` - Command enums, data structures, type definitions
- `PTSL.proto` - Protocol buffer definitions (multiple versioned .proto files available)

### Command Pattern (`Source/Commands/`)
Each PTSL command has its own `CppPTSLC_*.cpp` implementation file (e.g., `CppPTSLC_GetSessionName.cpp`, `CppPTSLC_CreateNewTracks.cpp`).

### Example Application (`examples/ptslcmd/`)
Command-line tool demonstrating SDK usage:
```sh
# Get help
ptslcmd -help

# List available commands
ptslcmd -list

# Register connection (required first)
ptslcmd RegisterConnection -json_request '{"company_name": "MyCompany", "application_name": "MyApp"}'

# Run command with session ID
ptslcmd GetSessionName -header:session_id "<session_id>"

# Run JSON script file
ptslcmd -file script.json
```

## Key Concepts

**Connection Registration**: Must call `RegisterConnection` before any other commands. Returns a `session_id` valid until Pro Tools exits.

**Server Port**: PTSL server binds to `localhost:31416` (local connections only).

**Request/Response Pattern**:
```cpp
PTSLC_CPP::ClientConfig config{ "localhost:31416", Mode::ProTools, SkipHostLaunch::No };
auto client = std::make_unique<PTSLC_CPP::CppPTSLClient>(config);

CppPTSLRequest request{ CommandId::GetSessionName, "{}" };
auto response = client->SendRequest(request).get();

if (response.GetStatus() == CommandStatusType::Completed) {
    // Use response.GetResponseBodyJson()
}
```

**JSON Script Format** (for ptslcmd -file):
```json
{
  "commands": [
    { "command_name": "RegisterConnection", "json_request": { "company_name": "X", "application_name": "Y" } },
    { "command_name": "GetSessionName" }
  ]
}
```

## Troubleshooting

Enable PTSL logging by placing `config.digitrace` next to Pro Tools executable:
```
DTF_PTSL=file@DTP_LOW
```

Logs written to:
- Mac: `~/Library/Logs/Avid`
- Windows: `%LocalAppData%\Avid\Logs`

Conan cache issues: Clear `~/.conan/data` and rebuild.
