# Copyright 2025 by Avid Technology, Inc.
include_guard()

if (APPLE AND PTSLC_CPP_BUILD_SHARED_LIBS)
    set(BUILDING_FRAMEWORK ON)
else()
    set(BUILDING_FRAMEWORK OFF)
endif()

# Setting up a custom target which should automatically select which install prefix to use depending on current build type.
setup_install_target(
    DEPENDS ${PROJECT_NAME}
    DEBUG_PREFIX "${PTSLC_CPP_INSTALL_DEBUG_PREFIX}"
    RELEASE_PREFIX "${PTSLC_CPP_INSTALL_RELEASE_PREFIX}"
)

# Set variables default values.
set(PROJECT_INSTALL_INCLUDEDIR "include")
set(PROJECT_INSTALL_LIBDIR "lib")
set(PROJECT_INSTALL_EXTERNAL_LIBDIR "lib")
set(PROJECT_INSTALL_BINDIR "bin")
set(PROJECT_INSTALL_FRAMEWORKDIR ".")

set(VERSION_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(CONFIG_INSTALL_DIR "CMake")
set(DEPENDENCIES_TARGET_FILE_BASE_NAME "${PROJECT_NAME}DependenciesTargets")

# Configure MacOS framework bundle.
if(BUILDING_FRAMEWORK)
    # Framework bundle structure
    # INSTALL_PREFIX/PTSLC_CPP.framework/
    #     Headers -> ./Versions/Current/Headers
    #     Resources -> ./Versions/Current/Resources
    #     PTSLC_CPP -> ./Versions/Current/PTSLC_CPP (dylib)
    #     Versions/
    #         Current -> ./A
    #         A/
    #             Headers/
    #                 Interface headers
    #             Resources/
    #                 Info.plist
    #                 CMake/
    #                     Configs, Targets files
    #             PTSLC_CPP
    #             CMakePackageHeaders/
    #                 PTSLC_CPP/ -> ./../Headers

    string(TIMESTAMP CURRENT_YEAR "%Y")
    set(COPYRIGHT "Copyright 1991-${CURRENT_YEAR} Avid Technology, Inc.")

    configure_file("${PLIST_TEMPLATE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" @ONLY)

    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            FRAMEWORK TRUE
            FRAMEWORK_VERSION A
            MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
            PUBLIC_HEADER "${PUBLIC_HEADERS}"
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
    )

    set(PROJECT_FRAMEWORKDIR "${PROJECT_NAME}.framework")
    set(PROJECT_CURRENTFRAMEWORKDIR "${PROJECT_FRAMEWORKDIR}/Versions/A")
    set(PROJECT_INSTALL_INCLUDEDIR "${PROJECT_CURRENTFRAMEWORKDIR}/CMakePackageHeaders")
    set(CONFIG_INSTALL_DIR "${PROJECT_FRAMEWORKDIR}/Resources/CMake")
endif()

# Install configuration.
install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    INCLUDES DESTINATION "${PROJECT_INSTALL_INCLUDEDIR}"
    LIBRARY DESTINATION "${PROJECT_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${PROJECT_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${PROJECT_INSTALL_BINDIR}"
    FRAMEWORK DESTINATION "${PROJECT_INSTALL_FRAMEWORKDIR}"
)

# Write package configuration files.
include(CMakePackageConfigHelpers)

write_basic_package_version_file("${VERSION_CONFIG_PATH}" COMPATIBILITY SameMajorVersion)
configure_package_config_file("${CMAKE_CONFIG_TEMPLATE_FILE}" "${CONFIG_PATH}" INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}")

# Install platform specific rules.
if(BUILDING_FRAMEWORK)
    # By default MacOS framework bundle puts all headers under NAME.framework/Headers
    # BUT all these headers are included via NAME prefix, i.e.
    # NAME.framework/Headers/File.h
    # #include <NAME/File.h>
    # To keep the behaviour consistent, CMake package config also provides the same path to includes
    # But without altering the original Apple suggested file tree structure - use symbolic link.
    install(CODE "
        execute_process(COMMAND mkdir -p \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_INCLUDEDIR}\")
        execute_process(COMMAND ln -sfh \"../Headers\" \"${PROJECT_NAME}\" WORKING_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_INSTALL_INCLUDEDIR}\")"
    )
else()
    install(
        FILES ${PUBLIC_HEADERS}
        DESTINATION "${PROJECT_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    )
endif()

# Bundle static external dependencies with the project. Only for static builds, for dynamic builds the dependencies are already compiled into the shared library.
if(NOT PTSLC_CPP_BUILD_SHARED_LIBS AND PTSLC_CPP_BUNDLE_STATIC_DEPENDENCIES)
    # Conan already provides a list of libraries and their paths ordered by the usage order in the conanbuildinfo.txt file.

    if(APPLE)
        set(DEPENDENCIES_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}")
    elseif(MSVC)
        set(DEPENDENCIES_ARCHITECTURES "${CMAKE_GENERATOR_PLATFORM}")
    endif()

    string(REPLACE "arm64" "armv8" DEPENDENCIES_ARCHITECTURES "${DEPENDENCIES_ARCHITECTURES}")
    string(REPLACE "x64" "x86_64" DEPENDENCIES_ARCHITECTURES "${DEPENDENCIES_ARCHITECTURES}")

    list(LENGTH DEPENDENCIES_ARCHITECTURES DEPENDENCIES_ARCHITECTURES_COUNT)

    # We support universal binary and multiconfiguration builds, so iterate over all available configurations and architectures.
    foreach(DEPENDENCIES_CONFIGURATION ${CMAKE_CONFIGURATION_TYPES})
        foreach(DEPENDENCIES_ARCHITECTURE ${DEPENDENCIES_ARCHITECTURES})
            if(DEPENDENCIES_ARCHITECTURES_COUNT GREATER 1)
                set(CURRENT_DEPENDENCIES_DIRECTORY "${DEPENDENCIES_DIRECTORY}/${DEPENDENCIES_ARCHITECTURE}")
            else()
                set(CURRENT_DEPENDENCIES_DIRECTORY "${DEPENDENCIES_DIRECTORY}")
            endif()

            set(DEPENDENCY_DATA_INI "${CURRENT_DEPENDENCIES_DIRECTORY}/conanbuildinfo-${DEPENDENCIES_ARCHITECTURE}-${DEPENDENCIES_CONFIGURATION}.txt")

            if(NOT EXISTS "${DEPENDENCY_DATA_INI}")
                message(WARNING "Could not find ${DEPENDENCY_DATA_INI}. Run the python script to install conan libraries. Skipping dependencies bundling for ${DEPENDENCIES_ARCHITECTURE} ${DEPENDENCIES_CONFIGURATION}.")
                continue()
            endif()

            # Collect all the link dependencies from the conanbuildinfo.txt file.

            read_lines_from_section(
                FILENAME "${DEPENDENCY_DATA_INI}"
                SECTION_NAME "libs"
                OUTPUT_LIST DEPENDENCIES_LIBS
            )
            read_lines_from_section(
                FILENAME "${DEPENDENCY_DATA_INI}"
                SECTION_NAME "system_libs"
                OUTPUT_LIST DEPENDENCIES_SYSTEM_LIBS
            )
            read_lines_from_section(
                FILENAME "${DEPENDENCY_DATA_INI}"
                SECTION_NAME "frameworks"
                OUTPUT_LIST DEPENDENCIES_FRAMEWORKS
            )
            read_lines_from_section(
                FILENAME "${DEPENDENCY_DATA_INI}"
                SECTION_NAME "libdirs"
                OUTPUT_LIST DEPENDENCIES_LIBDIRS
            )

            # Create a custom targets file that creates a single cmake target for all libs.

            set(DEPENDENCIES_NAMES "${DEPENDENCIES_LIBS}")
            set(SYSTEM_DEPENDENCIES_NAMES "${DEPENDENCIES_SYSTEM_LIBS}")
            set(FRAMEWORK_DEPENDENCIES_NAMES "${DEPENDENCIES_FRAMEWORKS}")
            set(DEPENDENCIES_ROOT_DIR "${PROJECT_INSTALL_EXTERNAL_LIBDIR}/${DEPENDENCIES_ARCHITECTURE}")
            set(DEPENDENCIES_TARGET_CMAKE_FILE "${CURRENT_DEPENDENCIES_DIRECTORY}/${DEPENDENCIES_TARGET_FILE_BASE_NAME}-${DEPENDENCIES_ARCHITECTURE}-${DEPENDENCIES_CONFIGURATION}.cmake")
            configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/CMake/DependenciesTargets.cmake.in"
                "${DEPENDENCIES_TARGET_CMAKE_FILE}"
                @ONLY
            )

            install(
                FILES "${DEPENDENCIES_TARGET_CMAKE_FILE}"
                DESTINATION "${CONFIG_INSTALL_DIR}"
                CONFIGURATIONS ${DEPENDENCIES_CONFIGURATION}
            )

            # Collect all the static libraries and install them.

            unset(DEPENDENCIES_LIBRARIES)
            foreach(DEPENDENCY_LIB ${DEPENDENCIES_LIBS})
                unset(DEPENDENCY_PATH)
                find_library(DEPENDENCY_PATH NAMES ${DEPENDENCY_LIB} PATHS ${DEPENDENCIES_LIBDIRS} NO_CACHE NO_DEFAULT_PATH)
                if (NOT DEPENDENCY_PATH)
                    message(FATAL_ERROR "Could not find ${DEPENDENCY_LIB} library in ${DEPENDENCIES_LIBDIRS}")
                endif()
                list(APPEND DEPENDENCIES_LIBRARIES ${DEPENDENCY_PATH})
            endforeach()

            install(
                FILES ${DEPENDENCIES_LIBRARIES}
                DESTINATION "${DEPENDENCIES_ROOT_DIR}"
                CONFIGURATIONS ${DEPENDENCIES_CONFIGURATION}
            )
        endforeach()
    endforeach()
endif()

install(
    FILES "${CONFIG_PATH}" "${VERSION_CONFIG_PATH}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${PROJECT_NAMESPACE}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)
