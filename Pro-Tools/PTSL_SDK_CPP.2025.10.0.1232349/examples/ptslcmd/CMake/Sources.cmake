# Copyright 2022-2025 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

list(APPEND HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PtslCmdCommandRunner.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PtslCmdCommandRequest.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PtslCmdCommandResult.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CommandLineParser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/FileScriptParser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PrintHelpHelpers.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Constants.h"
)


list(APPEND SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/ptslcmd.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PtslCmdCommandRunner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/CommandLineParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/FileScriptParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PrintHelpHelpers.cpp"

)

list(APPEND HEADERS_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)


if(PTSLC_CPP_DEVMODE)
    set(CMAKE_COMMON_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/CPP/CMake")
else()
    set(CMAKE_COMMON_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../CMake")
endif()
