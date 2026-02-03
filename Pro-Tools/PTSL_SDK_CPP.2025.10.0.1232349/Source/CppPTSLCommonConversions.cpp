// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Type conversions of types that are used by the PTSLC_CPP::CppPTSLClient.
 *
 * @deprecated Deprecated starting in Pro Tools 2024.10
 */

#include <optional>

#include <PTSL.pb.h>

#include "CppPTSLCommonConversions.h"

namespace PTSLC_CPP
{

#define DEFINE_PTSL_ENUM_CONVERSIONS_SPECIAL(ClientType, ProtoType)                                                    \
    template <>                                                                                                        \
    PTSLC_CPP_EXPORT std::string EnumToString<ClientType>(ClientType value)                                            \
    {                                                                                                                  \
        return GetStringFromEnum(ptsl::ProtoType##_descriptor(), value);                                               \
    }                                                                                                                  \
    template <>                                                                                                        \
    PTSLC_CPP_EXPORT std::optional<ClientType> StringToEnum<ClientType>(const std::string& value)                      \
    {                                                                                                                  \
        return GetEnumFromString<ClientType>(ptsl::ProtoType##_descriptor(), value);                                   \
    }

#define DEFINE_PTSL_ENUM_CONVERSIONS(Type) DEFINE_PTSL_ENUM_CONVERSIONS_SPECIAL(Type, Type)

    template <typename EnumT, typename DescriptorT>
    std::string GetStringFromEnum(const DescriptorT* descriptor, EnumT value)
    {
        if (!descriptor)
        {
            return {};
        }

        const auto descriptorValue = descriptor->FindValueByNumber(static_cast<int>(value));
        if (!descriptorValue)
        {
            return {};
        }

        return descriptorValue->name();
    }

    template <typename EnumT, typename DescriptorT>
    std::optional<EnumT> GetEnumFromString(const DescriptorT* descriptor, const std::string& value)
    {
        if (!descriptor)
        {
            return {};
        }

        const auto descriptorValue = descriptor->FindValueByName(value);
        if (!descriptorValue)
        {
            return {};
        }

        return static_cast<EnumT>(descriptorValue->number());
    }

    PTSLC_CPP_EXPORT std::vector<std::string> EnumValuesToStrings(const std::string& enumTypeName)
    {
        const std::string namespacePrefix = "ptsl.";

        const google::protobuf::EnumDescriptor* descriptor =
            google::protobuf::DescriptorPool::generated_pool()->FindEnumTypeByName(namespacePrefix + enumTypeName);

        if (!descriptor)
        {
            return {};
        }

        std::vector<std::string> result;
        for (int i = 0; i < descriptor->value_count(); ++i)
        {
            const google::protobuf::EnumValueDescriptor* value_descriptor = descriptor->value(i);
            result.push_back(value_descriptor->name());
        }

        return result;
    }

    DEFINE_PTSL_ENUM_CONVERSIONS(CommandId);
    DEFINE_PTSL_ENUM_CONVERSIONS(CommandErrorType);
    DEFINE_PTSL_ENUM_CONVERSIONS(TrackType);
    DEFINE_PTSL_ENUM_CONVERSIONS(TrackFormat);
    DEFINE_PTSL_ENUM_CONVERSIONS(TrackTimebase);
    DEFINE_PTSL_ENUM_CONVERSIONS(TrackOffsetOptions);
    DEFINE_PTSL_ENUM_CONVERSIONS(TimelineUpdateVideo);
    DEFINE_PTSL_ENUM_CONVERSIONS(TrackFromClipGroupExclusionReason);
    DEFINE_PTSL_ENUM_CONVERSIONS(TrackInsertionPoint);

    template <>
    PTSLC_CPP_EXPORT std::string EnumToString<CommandStatusType>(CommandStatusType value)
    {
        if (value == CommandStatusType::NoResponseReceived) // there is no such enum value in the proto file
        {
            return "NoResponseReceived";
        }

        return GetStringFromEnum(ptsl::TaskStatus_descriptor(), value);
    }

    template <>
    PTSLC_CPP_EXPORT std::optional<CommandStatusType> StringToEnum<CommandStatusType>(const std::string& value)
    {
        if (value == "NoResponseReceived") // there is no such enum value in the proto file
        {
            return CommandStatusType::NoResponseReceived;
        }

        return GetEnumFromString<CommandStatusType>(ptsl::TaskStatus_descriptor(), value);
    }

    template <>
    PTSLC_CPP_EXPORT std::string EnumToString<TripleBool>(TripleBool value)
    {
        return GetStringFromEnum(ptsl::TripleBool_descriptor(), value);
    }

    template <>
    PTSLC_CPP_EXPORT std::optional<TripleBool> StringToEnum<TripleBool>(const std::string& value)
    {
        // TODO: Replace with the usual macro when TripleBool's deprecated values are removed
        if (value == "None")
        {
            return TripleBool::TB_None;
        }
        if (value == "False")
        {
            return TripleBool::TB_False;
        }
        if (value == "True")
        {
            return TripleBool::TB_True;
        }

        return GetEnumFromString<TripleBool>(ptsl::TripleBool_descriptor(), value);
    }

} // namespace PTSLC_CPP
