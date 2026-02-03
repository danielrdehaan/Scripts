PTSL C++ Client Documentation      {#mainpage}
================================

[TOC]

## Welcome

The Pro Tools Scripting Library makes it possible for applications to interact with Pro Tools. PTSL supports RPC with any language supported by the gRPC protocol.

This package includes a reference C++ client implementation for PTSL as well as an example C++ client application, ptslcmd.

For a list of commands provided by the Pro Tools Scripting Library, see @ref ptsl_category_all

### License

All documents in the Pro Tools Scripting Library contain confidential information of Avid. Do not disclose to any third party. Use of the information contained in the Pro Tools Scripting Library is subject to an Avid SDK license. Questions regarding this agreement can be sent to Avid's Audio SDK team at audiosdk@avid.com.

## Getting Started

Follow this brief guide to get started with PTSL and use the ptslcmd example application to issue commands to Pro Tools.

### Prerequisites

Your system must meet the following requirements to build the PTSL C++ client and the ptslcmd example application successfully:

Mac:
- Python version >= 3.8
- XCode version >= 10.2.1 

Windows:
- Python version >= 3.8
- Visual Studio Community Edition (vs2017, vs2019, vs2022)
- Windows SDK 11 (10.0.22000)

Conan version 1.62.0 as provided in Config/requirements.txt or the latest 1.x version of Conan.

### Build the PTSL C++ Client Wrapper

The PTSL C++ Client Wrapper is a cmake project that creates a dynamic library for easy integration into your application.

Build the library using the setup/build_cpp_ptsl_sdk.py script. The script takes arguments for the project to build and the build configurations:

\code{.sh}

	python3 setup/build_cpp_ptsl_sdk.py --target ptsl.client.cpp --config Debug [--arch] [--os_version] [--vs_compiler]
\endcode

The compiled library is placed in the SDK's install directory.

This script sets up a python virtual environment (venv) and uses conan to pull in dependencies (including cmake) to that environment. Finally it uses cmake to perform the build. See the contents of the config directory if you'd like more details on how the build process works.

### Build and run the ptslcmd example application

The ptslcmd application provides an example of how to use the Client Wrapper library in a PTSL client. 

The build_cpp_ptsl_sdk.py script includes an option for building the example application:

\code{.sh}

	python3 setup/build_cpp_ptsl_sdk.py --target ptslcmd.sdk --config Debug [--arch] [--os_version] [--vs_compiler]
\endcode

ptslcmd provides a command line interface for executing various PTSL API commands. In addition to serving as an example of a simple client, ptslcmd can be used as a way to test out various API methods.

You can check an example of ptslcmd usage and command line arguments in ptslcmd.cpp

## Using the PTSL Client Wrapper

To use the PTSL C++ client wrapper in your application, create a @ref PTSLC_CPP::CppPTSLClient "CppPTSLClient" object. This class provides a C++ interface to the PTSL inter-process communication and commands.

This example code fragment shows how to instantiate the client using the @ref PTSLC_CPP::ClientConfig "ClientConfig" structure to configure and instantiate the client wrapper.

\code{.cpp}

	#include "CppPTSLClient.h"

	PTSLC_CPP::ClientConfig config
	{
		"localhost:31416",								// server address -- note the server port!
		ClientConfig::Mode::ProTools,					// turn off stand-alone mode -- look for a server instead of launching our own (see CppPTSLClient::LocateServer)
		ClientConfig::SkipHostLaunch::No				// do not skip auto launch -- try to launch Pro Tools if it is not running already
	};
	auto client = std::make_unique<PTSLC_CPP::CppPTSLClient>(config);
\endcode

### Establish a connection to Pro Tools

Pro Tools requires connection registration before any PTSL commands may be executed by your application.

Use the @ref ptsl::RegisterConnectionRequestBody "RegisterConnectionRequestBody" message to establish a connection to Pro Tools. This request takes 2 required string parameters: `company_name` and `application_name`.
See @ref ptsl::CId_RegisterConnection "RegisterConnection" for more details.

\code{.cpp}
PTSLC_CPP::CppPTSLRequest request{ PTSLC_CPP::CommandId::RegisterConnection, R"({ "company_name": "YOUR COMPANY NAME", "application_name": "YOUR APPLICATION NAME" })" };

auto response = client->SendRequest(request).get();
ParseResponse(registrationRsp); // see below
\endcode

Instead of manually creating a JSON string you can reuse protocol objects to generate it based on the PTSL.proto. For this option you need to generate `PTSL.pb.h` using the protobuf compiler.

\code{.cpp}
#include "PTSL.pb.h"

...

ptsl::RegisterConnectionRequestBody requestBody;
requestBody.set_company_name("YOUR COMPANY NAME");
requestBody.set_application_name("YOUR APPLICATION NAME");

google::protobuf::util::JsonOptions jOpts;
jOpts.always_print_primitive_fields = true;
jOpts.preserve_proto_field_names = true;

std::string json;
if (MessageToJsonString(requestBody, &json, jOpts).ok())
{
	auto response = client->SendRequest(PTSLC_CPP::CppPTSLRequest{ PTSLC_CPP::CommandId::RegisterConnection, json }).get();
}
else
{
	...
}
\endcode


If this command is successful it establishes a connection and provides an associated guid named `session_id`, which will be used in the request header for all subsequent commands. Note, the `session_id` persists throughout the life of the currently running instance of Pro Tools and has no correlation with opening and closing Pro Tools sessions. After Pro Tools exits you will need to re-register your client.

The client wrapper keeps track of your `session_id` for you and submits it in the request header with each of your commands, so you do not need to capture or track it yourself.

See Source/CppPTSLClient.cpp for more information about how the client wrapper tracks your `session_id`.

#### Local vs Remote Connection to PTSL Server

Note the PTSL service currently requires a local client for connection. This is because the server is bound to localhost. As a result, it does not support remote connections at this time.

#### PTSL Client Threading in AAX Plugins

For PTSL integration in AAX plugins, ensure PTSL client code runs in a separate thread. This avoids deadlocks from concurrent PTSL client code execution and incoming request servicing in the main Pro Tools thread.

### Issue a command to Pro Tools

In the steps below, the @ref ptsl::CId_ExportSessionInfoAsText "ExportSessionInfoAsText" command will be used as an example. This command exports session information into a text file.

The @ref ptsl::CId_ExportSessionInfoAsText "ExportSessionInfoAsText" command is configured using an @ref ptsl::ExportSessionInfoAsTextRequestBody "ExportSessionInfoAsTextRequestBody" data structure, a collection of parameters used to fill the client app's request to the PTSL server in Pro Tools.

This completed request can now be passed to the @ref PTSLC_CPP::CppPTSLClient::SendRequest "SendRequest" method.

\code{.cpp}
CppPTSLRequest request{ PTSLC_CPP::CommandId::ExportSessionInfoAsText, R"({
	"include_file_list": true,
	"include_clip_list": true,
	"include_markers": true,
	"include_plugin_list": true,
	"include_track_edls": true,
	"show_sub_frames": true,
	"include_user_timestamps": true,
	"track_list_type": "AllTracks",
	"fade_handling_type": "ShowCrossfades",
	"track_offset_options": "BarsBeats",
	"text_as_file_format": "UTF8",
	"output_type": "ESI_File",
	"output_path": "/path/to/session_info.txt"
})" };

auto response = client->SendRequest(request).get();
ParseResponse(response); // see below
\endcode

### Determine whether a command succeeded

It is important to check whether every command has succeeded by analyzing the @ref PTSLC_CPP::CppPTSLResponse "CppPTSLResponse" it returns. Command failures may occur for a number of reasons, and your client application should be designed to accept and recover from a command error. Depending on the error condition, a re-try may succeed.

This example shows a test implementation checking a command response for success.

\code{.cpp}
void ParseResponse(const PTSLC_CPP::CppPTSLResponse& rsp)
{
	if (rsp.GetStatus() == PTSLC_CPP::CommandStatusType::Completed)
	{
		std::cout << "Command completed successfully!\n" << rsp.GetResponseBodyJson() << "\n";
	}
	else if (rsp.GetStatus() == PTSLC_CPP::CommandStatusType::Failed)
	{
		std::cout << "Command failed!\n" << rsp.GetResponseErrorJson() << "\n";
		// or
		for (const auto& error : rsp.GetResponseErrorList().errors)
		{
			std::cout << (error->isWarning ? "Warning " : "Error ") << static_cast<int>(error->errorType) << ": " << error->errorMessage << "\n";
		}
	}
}
\endcode

## Versioning and compatibility

Compatibility information for specific releases is noted in the 'Release Notes' tab.

If a PTSL client application attempts to connect to an incompatible version of Pro Tools, a @ref PTSLC_CPP::CommandErrorType::SDK_VersionMismatch "SDK_VersionMismatch" error will be returned.

For more information about versioning, see @ref PTSL_Versions.h

## Troubleshooting

<!-- TODO: Move this to its own page -->

### General FAQ

For answers to the most frequently asked questions, see the <a href="https://kb.avid.com/pkb/articles/FAQ/Pro-Tools-Scripting-SDK-FAQ" target="_blank">Pro Tools Scripting SDK FAQ</a>.

### Pro Tools Logs

When troubleshooting issues with PTSL it is often useful to review the Pro Tools application logs. Pro Tools log files are written to ~Library/Logs/Avid on Mac and \%LocalAppData\%\\Avid\\Logs on Windows.

To enable PTSL-specific logging, place a file named "config.digitrace" next to the Pro Tools application bundle (Mac) or executable (Windows) containing the following line:

```
DTF_PTSL=file@DTP_LOW
```

If the PTSL tracing has been enabled properly you should see `DTF_PTSL@DTP_LOW(0e28)` in the log file's preamble and logs such as "Incoming PTSL command: {...}" when Pro Tools receives commands from the client. All PTSL logging traces will include `0e28` in the "Facility" column, so you can use that identifier to quickly search the log file for PTSL related information.


### Conan errors when running the build script

If you see an error referencing a problem with a Conan package when running the setup/build_cpp_ptsl_sdk.py script then follow the steps below.

An example error type that you may see:

\code{.unparsed}

Error loading conanfile ... Current Conan version does not satisfy the defined one.
\endcode

As a quick fix, it is sometimes possible to resolve this problem by clearing the conan cache at ~/.conan/data, then re-running the build script. This will essentially delete all of the PTSL dependencies that have been downloaded already, and this will fix the issue if the underlying problem is a corrupt or invalid package in the cache.

If clearing the cache does not work then the next step is to update the requirements.txt file. This will ensure that both the Conan package itself and all of its dependencies are up-to-date with the requirements from the public conan registry. All of the next steps should be done in the same folder:

1. Rename the existing requirements.txt file to something else.
2. Create a virtual environment (venv) to store new dependencies \code{.unparsed} python -m venv .venv \endcode
3. Activate the venv \code{.unparsed} source .venv/bin/activate \endcode
4. Install the latest Conan package \code{.unparsed} pip install conan \endcode
5. Install CMake \code{.unparsed} pip install cmake==3.25.0 \endcode
6. Export to a new requirements.txt file \code{.unparsed} pip freeze > requirements.txt \endcode
7. Finally, attempt to rebuild using the build script

Another class of errors may look like:

\code{.unparsed} 

PtslClient/None: 'settings.compiler' value not defined

\endcode

#### Cause

Running the build script without the necessary build tools can lead to persistent errors. This happens because Conan creates a default profile that lacks complete information during the initial run.

#### Resolution

To fix this, delete the default Conan profile and re-run the build. Conan will then create a new profile that includes the build tools you’ve installed.

### Requests are refused

Verify that your client application has issued a @ref ptsl::CId_RegisterConnection "RegisterConnection" and that the result was successful.

### Unexpected errors

It is not guaranteed that a command issued by your application will succeed even if it is well-formed. Errors on the Pro Tools application side can occur during normal operation. Your application should be robust against such failures, for example by incorporating a retry sequence for failing commands and by providing suitable UI to prompt the user when a request has failed repeatedly.
