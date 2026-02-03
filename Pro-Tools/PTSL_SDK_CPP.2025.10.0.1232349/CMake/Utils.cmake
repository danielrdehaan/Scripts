# Copyright 2022-2025 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

# Generate .h/.cpp sources based on the source .proto file.
function(generate_proto_files)
    cmake_parse_arguments(PARSE_ARGV 0 GEN_PROTO "NO_PLUGIN" "FILE;OUTPUT_DIRECTORY;PROTOC;PLUGIN;OUTPUT_HEADERS_VARIABLE;OUTPUT_SOURCES_VARIABLE" "")

    # If compilers aren't set - try to find them in PATH
    if (NOT GEN_PROTO_PROTOC)
        find_program(GEN_PROTO_PROTOC protoc NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)

        if(NOT GEN_PROTO_PROTOC)
            message(FATAL_ERROR "Can't find protoc. Pass path to it as a function argument or update the PATH environment variable")
        endif()
    endif()

    if (NOT GEN_PROTO_NO_PLUGIN)
        if (NOT GEN_PROTO_PLUGIN)
            find_program(GEN_PROTO_PLUGIN grpc_cpp_plugin NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)

            if(NOT GEN_PROTO_PLUGIN)
                message(FATAL_ERROR "Can't find grpc plugin. Pass path to it as a function argument or update the PATH environment variable")
            endif()
        endif()
    endif()

    get_filename_component(PROTO_DIR "${GEN_PROTO_FILE}" DIRECTORY)
    get_filename_component(PROTO_BASENAME "${GEN_PROTO_FILE}" NAME_WLE)

    message(STATUS "Adding custom command for \"${GEN_PROTO_FILE}\" using base name, \"${PROTO_BASENAME}\"")

    if (GEN_PROTO_NO_PLUGIN)
        set(HEADERS "${GEN_PROTO_OUTPUT_DIRECTORY}/${PROTO_BASENAME}.pb.h")
        set(SOURCES "${GEN_PROTO_OUTPUT_DIRECTORY}/${PROTO_BASENAME}.pb.cc")

        add_custom_command(
            OUTPUT ${HEADERS} ${SOURCES}
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${GEN_PROTO_OUTPUT_DIRECTORY}"
            COMMAND
                    "${GEN_PROTO_PROTOC}"
                    "-I=${PROTO_DIR}"
                    "--cpp_out=${GEN_PROTO_OUTPUT_DIRECTORY}"
                    "${GEN_PROTO_FILE}"
            DEPENDS "${GEN_PROTO_FILE}"
            COMMENT "Generating PTSL protobuf code for the cpp"
            VERBATIM
        )
    else()
        set(HEADERS "${GEN_PROTO_OUTPUT_DIRECTORY}/${PROTO_BASENAME}.pb.h;${GEN_PROTO_OUTPUT_DIRECTORY}/${PROTO_BASENAME}.grpc.pb.h")
        set(SOURCES "${GEN_PROTO_OUTPUT_DIRECTORY}/${PROTO_BASENAME}.pb.cc;${GEN_PROTO_OUTPUT_DIRECTORY}/${PROTO_BASENAME}.grpc.pb.cc")

        add_custom_command(
            OUTPUT ${HEADERS} ${SOURCES}
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${GEN_PROTO_OUTPUT_DIRECTORY}"
            COMMAND
                    "${GEN_PROTO_PROTOC}"
                    "-I=${PROTO_DIR}"
                    "--plugin=protoc-gen-grpc=${GEN_PROTO_PLUGIN}"
                    "--cpp_out=${GEN_PROTO_OUTPUT_DIRECTORY}"
                    "--grpc_out=${GEN_PROTO_OUTPUT_DIRECTORY}"
                    "${GEN_PROTO_FILE}"
            DEPENDS "${GEN_PROTO_FILE}"
            COMMENT "Generating PTSL protobuf code for the cpp"
            VERBATIM
        )
    endif()

    set(${GEN_PROTO_OUTPUT_HEADERS_VARIABLE} "${HEADERS}" PARENT_SCOPE)
    set(${GEN_PROTO_OUTPUT_SOURCES_VARIABLE} "${SOURCES}" PARENT_SCOPE)
endfunction()

# Generate list of symbols which should not be exported in the result library.
# This applies to static libraries which are not affected by current visibility setting.
# TODO another solution:
# Use ld -hidden-l instead of -l
# Could be implemented using CMake 3.24
function(hide_dependencies_symbols)
    cmake_parse_arguments(PARSE_ARGV 0 HIDE_SYMBOLS "" "PROJECT;DEPENDENCIES_DIRECTORY;MAP_FILE" "")

    file(REMOVE "${HIDE_SYMBOLS_MAP_FILE}")
    file(GLOB_RECURSE DEPENDENCY_ARCHIVES "${HIDE_SYMBOLS_DEPENDENCIES_DIRECTORY}/*.a")
    foreach(ARCHIVE ${DEPENDENCY_ARCHIVES})
        execute_process(
            # For the specified library get global symbols in a readable form.
            COMMAND nm -gjUo ${ARCHIVE}
            # Read the last column - the symbol itself.
            COMMAND awk "{print $NF}"
            COMMAND_ERROR_IS_FATAL ANY
            OUTPUT_VARIABLE UNEXPORTED_SYMBOLS
        )

        file(APPEND "${HIDE_SYMBOLS_MAP_FILE}" "${UNEXPORTED_SYMBOLS}")
    endforeach()

    # man ld
    # The specified filename contains a list of global symbol names that will not remain as global symbols in the output file.
    # The symbols will be treated as if they were marked as __private_extern__ (aka visibility=hidden) and will not be global in the output file.
    #
    # Using -Wl explicitly because LINKER: chooses -Xlinker and as a result CMake generates additional strange path that includes that flag.
    target_link_options(${HIDE_SYMBOLS_PROJECT} PRIVATE "-Wl,-unexported_symbols_list,${HIDE_SYMBOLS_MAP_FILE}")
endfunction()

# Creates PTSL_INSTALL target that invokes CMake's INSTALL but allows to customize install directory based on current configuration.
function(setup_install_target)
    cmake_parse_arguments(PARSE_ARGV 0 INSTALL "" "DEPENDS;DEBUG_PREFIX;RELEASE_PREFIX" "")

    add_custom_target(PTSL_INSTALL
        COMMAND "${CMAKE_COMMAND}" --install "${CMAKE_CURRENT_BINARY_DIR}" --config $<CONFIG> --prefix "$<$<CONFIG:Debug>:${INSTALL_DEBUG_PREFIX}>" "$<$<CONFIG:Release>:${INSTALL_RELEASE_PREFIX}>"
        DEPENDS ${INSTALL_DEPENDS}
    )
endfunction()

# Read lines from a section in an INI file.
function(read_lines_from_section)
    set(options)
    set(oneValueArgs FILENAME SECTION_NAME OUTPUT_LIST)
    set(multiValueArgs)
    cmake_parse_arguments(READ_LINES_FROM_SECTION "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(STRINGS
        "${READ_LINES_FROM_SECTION_FILENAME}"
        INI_DATA
    )

    set(IN_SECTION FALSE)
    foreach(LINE IN LISTS INI_DATA)
        if (LINE STREQUAL "[${READ_LINES_FROM_SECTION_SECTION_NAME}]")
            set(IN_SECTION TRUE)
            continue()
        endif()
        if (IN_SECTION)
            if (LINE STREQUAL "")
                set(IN_SECTION FALSE)
                continue()
            endif()
            list(APPEND SECTION_LIST "${LINE}")
        endif()
    endforeach()

    set(${READ_LINES_FROM_SECTION_OUTPUT_LIST} "${SECTION_LIST}" PARENT_SCOPE)
endfunction()
