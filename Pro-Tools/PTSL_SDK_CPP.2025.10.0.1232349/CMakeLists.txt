# Copyright 2022-2023 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

cmake_minimum_required (VERSION 3.25 FATAL_ERROR)

# MSVC flags abstractions.
cmake_policy(SET CMP0091 NEW)

project(PTSLC_CPP VERSION 1.0.0 LANGUAGES CXX)
set(PROJECT_NAMESPACE "${PROJECT_NAME}::")

##### CMake input options.
option(PTSLC_CPP_DEVMODE "Use original source tree for project generation instead of a copy" OFF)
option(PTSLC_CPP_BUILD_SHARED_LIBS "Build the shared library" ON)
option(PTSLC_CPP_BUNDLE_STATIC_DEPENDENCIES "Bundle static dependencies with the package" OFF)
set(PTSLC_CPP_INSTALL_DEBUG_PREFIX "" CACHE STRING "CMAKE_INSTALL_PREFIX for Debug")
set(PTSLC_CPP_INSTALL_RELEASE_PREFIX "" CACHE STRING "CMAKE_INSTALL_PREFIX for Release")

if (PTSLC_CPP_BUILD_SHARED_LIBS AND PTSLC_CPP_BUNDLE_STATIC_DEPENDENCIES)
    message(WARNING "PTSLC_CPP_BUNDLE_STATIC_DEPENDENCIES is ignored for shared libraries: static dependencies are compiled into the shared library")
    set(PTSLC_CPP_BUNDLE_STATIC_DEPENDENCIES OFF)
endif()

##### Setup required variables.

set(GENERATED_FILES_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Generated")
set(DEPENDENCIES_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Dependencies")
set(DEPENDENCIES_TARGET_NAME "Dependencies")
set(DEPENDENCIES_TARGET_FULL_NAME "${PROJECT_NAMESPACE}Dependencies")

##### Setup source variables.
include(CMake/Sources.cmake)

##### Include required modules.
include(GenerateExportHeader)
include(${CMAKE_COMMON_SCRIPTS_DIR}/Utils.cmake)

##### Dependencies setup.

# Forcing protoc, grpc_cpp_plugin search to save the path in the cache.
# At the momemt conan uses only ENV option for find_program postponing usage of the CMakeToolchain until v2.0
find_program(GRPC_CPP_PLUGIN_PROGRAM NAMES grpc_cpp_plugin)
find_program(PROTOC_PROGRAM NAMES protoc)

find_package(OpenSSL REQUIRED CONFIG)
find_package(gRPC REQUIRED CONFIG)
find_package(protobuf REQUIRED CONFIG)
find_package(date REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)

##### Library configuration.

if (PTSLC_CPP_BUILD_SHARED_LIBS)
    set(LIBRARY_TYPE SHARED)
else()
    set(LIBRARY_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} "")
add_library(${PROJECT_NAMESPACE}${PROJECT_NAME} ALIAS ${PROJECT_NAME})

foreach(PROTO_FILE ${PROTO_FILES})
    generate_proto_files(
        FILE ${PROTO_FILE}
        OUTPUT_DIRECTORY "${GENERATED_FILES_DIRECTORY}"
        PROTOC "$<TARGET_FILE:protobuf::protoc>"
        PLUGIN "$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
        OUTPUT_HEADERS_VARIABLE CURRENT_PROTO_HEADERS
        OUTPUT_SOURCES_VARIABLE CURRENT_PROTO_SOURCES
    )
    list(APPEND PROTO_HEADERS ${CURRENT_PROTO_HEADERS})
    list(APPEND PROTO_SOURCES ${CURRENT_PROTO_SOURCES})
endforeach()

# Set project sources.
target_sources(
    ${PROJECT_NAME}
    PRIVATE ${HEADERS}
    PRIVATE ${PROTO_HEADERS}
    PRIVATE ${SOURCES}
    PRIVATE ${COMMANDS_SOURCES}
    PRIVATE ${PROTO_SOURCES}
    PRIVATE ${PROTO_FILE}
)

# Set project parameters.
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        LINKER_LANGUAGE CXX
        CXX_VISIBILITY_PRESET hidden
)

if(WIN32)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
        CMAKE_CXX_STANDARD_LIBRARIES "$(CMAKE_CXX_STANDARD_LIBRARIES) %(AdditionalDependencies)"
    )
endif()

# Set platform specific properties.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /EHsc /MP)
endif()

# Add preprocessor definitions.
if (PTSLC_CPP_BUILD_SHARED_LIBS)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE PTSLC_CPP_EXPORTS
    )
else()
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE PTSLC_CPP_STATIC_DEFINE
    )
endif()

if(MSVC)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE _WINDOWS
        PRIVATE _WIN32_WINNT=0x600
        PRIVATE _USRDLL
        PRIVATE UNICODE
        PRIVATE _UNICODE
    )
endif()

# Add paths to include files into the target project.
target_include_directories(
    ${PROJECT_NAME}
    # Allow for private cases + "in solution" direct linking. Forbid path for installation.
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../Common/CPP/Source>"
    PUBLIC "$<BUILD_INTERFACE:${GENERATED_FILES_DIRECTORY}>"
)

# Link dependencies.
# Wrap conan dependencies in the build interface as those are manually assembled into a different target used with install interface.
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE $<BUILD_INTERFACE:$<TARGET_NAME_IF_EXISTS:OpenSSL::OpenSSL>>
    PRIVATE $<BUILD_INTERFACE:$<TARGET_NAME_IF_EXISTS:openssl::openssl>>
    PRIVATE $<BUILD_INTERFACE:grpc::grpc>
    PRIVATE $<BUILD_INTERFACE:protobuf::protobuf>
    PRIVATE $<BUILD_INTERFACE:date::date>
    PRIVATE $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
    PRIVATE $<INSTALL_INTERFACE:$<$<BOOL:${PTSLC_CPP_BUNDLE_STATIC_DEPENDENCIES}>:${PROJECT_NAMESPACE}${DEPENDENCIES_TARGET_NAME}>>
)

# Generate library export header. Provides macro to wrap import/export attributes.
generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME "${LIBRARY_EXPORT_HEADER}"
)

##### Configure IDE tree view.
source_group("Header Files" FILES ${HEADERS} ${PROTO_HEADERS})
source_group("Source Files" FILES ${SOURCES} ${PROTO_SOURCES})
source_group("Source Files/Commands" FILES ${SOURCES})
source_group("" FILES ${PROTO_FILE})

##### Installation instructions.
include(CMake/Install.cmake)
