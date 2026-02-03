# Copyright 2021-2023 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

cmake_minimum_required (VERSION 3.25 FATAL_ERROR)

# MSVC flags abstractions.
cmake_policy(SET CMP0091 NEW)

project(ptslcmd VERSION 1.0.0 LANGUAGES CXX)

##### CMake input parameters.
option(MACOS_PTSLC_CPP_USE_FRAMEWORK "Use MacOS framework bundle to link PTSL C++ client instead of the CMake config. MacOS only." OFF)
option(PTSLC_CPP_DEVMODE "Use original source tree for project generation instead of a copy" OFF)
set(INSTALL_DEBUG_PREFIX "" CACHE STRING "CMAKE_INSTALL_PREFIX for Debug")
set(INSTALL_RELEASE_PREFIX "" CACHE STRING "CMAKE_INSTALL_PREFIX for Release")
set(PTSLC_CPP_DEBUG_ROOT "" CACHE STRING "PTSLC_CPP_ROOT for Debug. Use it to forcefully point PTSLC_CPP location for Debug (Optional)")
set(PTSLC_CPP_RELEASE_ROOT "" CACHE STRING "PTSLC_CPP_ROOT for Release. Use it to forcefully point PTSLC_CPP location for Release (Optional)")
set(EXECUTABLE_NAME "ptslcmd" CACHE STRING "Override the default executable name")

##### Dependencies setup.
if(APPLE AND MACOS_PTSLC_CPP_USE_FRAMEWORK)
    set(PTSLC_CPP_USING_FRAMEWORK TRUE)
    set(PTSLC_CPP_NAME "PTSLC_CPP")
else()
    set(PTSLC_CPP_USING_FRAMEWORK FALSE)
    find_package(PTSLC_CPP ${PROJECT_VERSION} REQUIRED CONFIG)
endif()

find_package(GTest REQUIRED CONFIG)
find_package(boost REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)
find_package(date REQUIRED CONFIG)

##### Setup source variables.
include(cmake/Sources.cmake)

##### Include required modules.
include(${CMAKE_COMMON_SCRIPTS_DIR}/Utils.cmake)

##### Executable configuration.
add_executable(${PROJECT_NAME} "")

# Set project sources.
target_sources(
    ${PROJECT_NAME}
    PRIVATE ${HEADERS}
    PRIVATE ${SOURCES}
)

# Set project parameters.

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        LINKER_LANGUAGE CXX
        OUTPUT_NAME "${EXECUTABLE_NAME}"
)

if(MSVC)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
            CMAKE_CXX_STANDARD_LIBRARIES "$(CMAKE_CXX_STANDARD_LIBRARIES) %(AdditionalDependencies)"
    )
elseif(APPLE)
    # Set RPATH for dynamic linker.
    # BUILD_WITH_INSTALL_RPATH is a temporary(read as permanent) fix until: https://gitlab.kitware.com/cmake/cmake/-/issues/17995 or https://gitlab.kitware.com/cmake/cmake/-/issues/21854
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
    )
    set_property(
        TARGET ${PROJECT_NAME}
        APPEND PROPERTY
            INSTALL_RPATH "@executable_path" "$<$<CONFIG:Debug>:${PTSLC_CPP_DEBUG_ROOT}>" "$<$<CONFIG:Release>:${PTSLC_CPP_RELEASE_ROOT}>"
        )
endif()

# Set platform specific properties.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /EHsc /MP)
endif()

# Add preprocessor definitions.
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MAC_VERSION=1)
elseif(WIN32)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE WIN_VERSION=1
        PRIVATE WINDOWS_VERSION
        PRIVATE -D_WIN32_WINNT=0x600
        PRIVATE -DUNICODE
        PRIVATE -D_UNICODE
    )
endif()

# Add paths to include files into the target project.
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${HEADERS_DIRECTORIES}
)

# Link dependencies.
target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::GTest
    PRIVATE boost::boost
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE date::date
)

# Link PTSL C++ client either as framework or using CMake.
if(PTSLC_CPP_USING_FRAMEWORK)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework ${PTSLC_CPP_NAME}")

    # Specify the path where to search the framework.
    target_compile_options(${PROJECT_NAME}
        PRIVATE "$<$<CONFIG:Debug>:-F${PTSLC_CPP_DEBUG_ROOT}>"
        PRIVATE "$<$<CONFIG:Release>:-F${PTSLC_CPP_RELEASE_ROOT}>"
    )
    target_link_options(${PROJECT_NAME}
        PRIVATE "$<$<CONFIG:Debug>:LINKER:-F${PTSLC_CPP_DEBUG_ROOT}>"
        PRIVATE "$<$<CONFIG:Release>:LINKER:-F${PTSLC_CPP_RELEASE_ROOT}>"
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE PTSLC_CPP::PTSLC_CPP)
endif()

##### Installation instructions.
setup_install_target(
    DEPENDS ${PROJECT_NAME}
    DEBUG_PREFIX "${INSTALL_DEBUG_PREFIX}"
    RELEASE_PREFIX "${INSTALL_RELEASE_PREFIX}"
)

if(APPLE)
    set(RUNTIME_DIR ".")
elseif(WIN32)
    set(RUNTIME_DIR "bin")
endif()

# Set up executable output directory.
install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${RUNTIME_DIR}"
)

if(WIN32)
    install(
        FILES "$<TARGET_FILE:PTSLC_CPP::PTSLC_CPP>"
        DESTINATION "${RUNTIME_DIR}"
    )
endif()
