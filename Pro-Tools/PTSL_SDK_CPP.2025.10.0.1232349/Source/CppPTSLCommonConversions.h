// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Type conversions of types that are used by the PTSLC_CPP::CppPTSLClient.
 *
 * @deprecated Deprecated starting in Pro Tools 2024.10
 */

#pragma once

#include <optional>
#include <string>

#include "CppPTSLCommon.h"
#include "PtslCCppExport.h"

namespace PTSLC_CPP
{
    template <typename T>
    std::string EnumToString(T value)
    {
        static_assert(sizeof(T) == -1, "Please specialize this template for enum T");
    }

    template <typename T>
    std::optional<T> StringToEnum(const std::string& value)
    {
        static_assert(sizeof(T) == -1, "Please specialize this template for enum T");
    }

    PTSLC_CPP_EXPORT std::vector<std::string> EnumValuesToStrings(const std::string& enumTypeName);

// Enumerations, synchronized with proto structures.
#define DECLARE_PTSL_ENUM_CONVERSIONS(Type)                                                                            \
    template <>                                                                                                        \
    PTSLC_CPP_EXPORT std::string EnumToString<Type>(Type value);                                                       \
    template <>                                                                                                        \
    PTSLC_CPP_EXPORT std::optional<Type> StringToEnum<Type>(const std::string& value)

    DECLARE_PTSL_ENUM_CONVERSIONS(CommandId);
    DECLARE_PTSL_ENUM_CONVERSIONS(CommandStatusType);
    DECLARE_PTSL_ENUM_CONVERSIONS(CommandErrorType);
    DECLARE_PTSL_ENUM_CONVERSIONS(TrackType);
    DECLARE_PTSL_ENUM_CONVERSIONS(TrackFormat);
    DECLARE_PTSL_ENUM_CONVERSIONS(TrackTimebase);
    DECLARE_PTSL_ENUM_CONVERSIONS(TimelineUpdateVideo);
    DECLARE_PTSL_ENUM_CONVERSIONS(TripleBool);
    DECLARE_PTSL_ENUM_CONVERSIONS(TrackOffsetOptions);
    DECLARE_PTSL_ENUM_CONVERSIONS(TrackFromClipGroupExclusionReason);
    DECLARE_PTSL_ENUM_CONVERSIONS(TrackInsertionPoint);

#undef DECLARE_PTSL_ENUM_CONVERSIONS

} // namespace PTSLC_CPP
