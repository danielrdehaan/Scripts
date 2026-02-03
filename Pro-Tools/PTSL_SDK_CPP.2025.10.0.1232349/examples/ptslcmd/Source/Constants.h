// Copyright 2024-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

#pragma once

/**
 * Constants for the PTSL Cmd arguments and JSON file script.
 */
class Constants
{
public:
    // PTSL Cmd argument names:
    inline static const std::string JSON_REQUEST_CMD_ARG = "-json_request";
    inline static const std::string JSON_HEADER_CMD_ARG = "-json_header";
    inline static const std::string HELP_CMD_ARG = "-help";
    inline static const std::string LIST_CMD_ARG = "-list";
    inline static const std::string FILE_CMD_ARG = "-file";
    inline static const std::string SESSION_ID_CMD_ARG = "-header:session_id";
    inline static const std::string ENCRYPTION_KEY_ARG = "-encryption_key";

    // PTSL Cmd argument parsing:
    inline static const char PARAMETER_PREFIX = '-';
    inline static const std::string PARAMETERLESS_ARGS_KEY = "(-)";

    // JSON file script tags:
    inline static const std::string COMMANDS_JSON_TAG = "commands";
    inline static const std::string COMMAND_NAME_JSON_TAG = "command_name";
    inline static const std::string COMMAND_VERSION_JSON_TAG = "version";
    inline static const std::string COMMAND_REQUEST_BODY_JSON_TAG = "json_request";
    inline static const std::string COMMAND_HEADER_JSON_TAG = "json_header";
    inline static const std::string COMMAND_ENCRYPTION_KEY = "encryption_key";

    // JSON response tags:
    inline static const std::string RESPONSE_HEADER = "header";
    inline static const std::string RESPONSE_BODY = "responseBodyJson";
    inline static const std::string RESPONSE_ERROR = "responseErrorJson";
    inline static const std::string RESPONSE_HEADER_TASK_ID = "taskId";
    inline static const std::string RESPONSE_HEADER_COMMAND = "command";
    inline static const std::string RESPONSE_HEADER_STATUS = "status";
    inline static const std::string RESPONSE_HEADER_PROGRESS = "progress";
    inline static const std::string RESPONSE_HEADER_VERSION = "version";
    inline static const std::string RESPONSE_HEADER_VERSION_MINOR = "versionMinor";
    inline static const std::string RESPONSE_HEADER_VERSION_REVISION = "versionRevision";
    inline static const std::string RESPONSE_HEADER_VERSIONED_RESPONSE_HEADER_JSON = "versionedResponseHeaderJson";
};
