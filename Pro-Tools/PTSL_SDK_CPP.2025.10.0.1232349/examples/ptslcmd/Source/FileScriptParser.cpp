// Copyright 2023-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the ptslcmd file script parser.
 */

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <nlohmann/json.hpp>
#include <optional>

#include "Constants.h"
#include "FileScriptParser.h"
#include "PtslCmdCommandRequest.h"

using boost::fusion::operator<<;
using json = nlohmann::json;

BOOST_FUSION_ADAPT_STRUCT(PtslCmdCommandRequest, CommandName, Version, RequestBodyJson)
BOOST_FUSION_ADAPT_STRUCT(PtslCmdFileScript, CommandRequests)

namespace comments
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using ascii::char_;
        using x3::double_;
        using x3::int_;
        using x3::lexeme;
        using x3::lit;

        // Filter out all these types of comments
        auto cppcomment = x3::omit[x3::lit("//") >> *(x3::char_ - (x3::eol | x3::eoi))];
        auto ccomment = x3::omit[x3::lit("/*") > *(x3::char_ - x3::lit("*/")) > x3::lit("*/")];

        // Match quoted string to keep "comments" inside them.
        auto qstring_simple = x3::lexeme[x3::char_('"') > *(x3::string("\\\"") | ~x3::char_('"')) > x3::char_('"')];

        const x3::rule<struct comments_filter, std::vector<std::string>> comments_filter = "comments_filter";
        auto comments_filter_def =
            *(qstring_simple | cppcomment | ccomment | +(char_ - (x3::lit('"') | x3::lit("//") | x3::lit("/*"))));

        BOOST_SPIRIT_DEFINE(comments_filter);
    } // namespace parser
} // namespace comments

//
// Namespace `commands` was removed (ashevche, 26.07.2024). Find it in Git history if needed.
//

std::optional<PtslCmdFileScript> FileScriptParser::Parse(const std::string& text)
{
    namespace x3 = boost::spirit::x3;

    try
    {
        auto doParse = [](const auto& str, auto filter, auto& result)
        {
            auto iter = str.begin();
            auto end = str.end();

            x3::error_handler<std::string::const_iterator> errorHandler(iter, end, std::cout);

            // Pass error handler to use it in on_error
            const auto parser = x3::with<x3::error_handler_tag>(std::ref(errorHandler))[filter];

            const bool success = x3::parse(iter, end, parser, /*out*/ result);
            const bool reachedEnd = (iter == end);
            const bool allFine = success && reachedEnd;

            if (!allFine)
            {
                throw std::runtime_error("Couldn't process input: " + std::string { iter, end });
            }
        };

        auto doParseJsonScript = [](const auto& str, auto& result)
        {
            json jObject = json::parse(str);

            int currCmdNo = 0;
            for (const auto& command : jObject[Constants::COMMANDS_JSON_TAG])
            {
                ++currCmdNo;

                if (!command.contains(Constants::COMMAND_NAME_JSON_TAG))
                {
                    throw std::runtime_error(
                        "In the JSON script, command #" + std::to_string(currCmdNo) + " lacks a specified name");
                }

                PtslCmdCommandRequest commandRequest;
                commandRequest.CommandName = command[Constants::COMMAND_NAME_JSON_TAG];

                if (command.contains(Constants::COMMAND_VERSION_JSON_TAG))
                {
                    auto versionString = command[Constants::COMMAND_VERSION_JSON_TAG].get<std::string>();
                    std::vector<std::string> versionParts;
                    boost::split(versionParts, versionString, boost::is_any_of("."));
                    std::get<0>(commandRequest.Version) = std::stoi(versionParts[0]);
                    std::get<1>(commandRequest.Version) = versionParts.size() > 1 ? std::stoi(versionParts[1]) : 0;
                    std::get<2>(commandRequest.Version) = versionParts.size() > 2 ? std::stoi(versionParts[2]) : 0;
                }

                if (command.contains(Constants::COMMAND_REQUEST_BODY_JSON_TAG))
                {
                    json jBody = command[Constants::COMMAND_REQUEST_BODY_JSON_TAG];
                    commandRequest.RequestBodyJson = jBody.dump();
                }

                if (command.contains(Constants::COMMAND_HEADER_JSON_TAG))
                {
                    json jHeader = command[Constants::COMMAND_HEADER_JSON_TAG];
                    commandRequest.VersionedHeader = jHeader.dump();
                }

                if (command.contains(Constants::COMMAND_ENCRYPTION_KEY))
                {
                    json jEncryptionKey = command[Constants::COMMAND_ENCRYPTION_KEY];
                    commandRequest.EncryptionKey = nlohmann::json::parse(jEncryptionKey.dump()).get<std::string>();
                }

                result.CommandRequests.push_back(commandRequest);
            }
        };

        // Stage 1: remove the comments and regenerate a new input string.
        std::cout << "Parsing command script. Stage 1: removing comments..." << std::endl;
        std::vector<std::string> preParseResult;
        doParse(text, comments::parser::comments_filter, /*out*/ preParseResult);

        auto filteredStr = boost::algorithm::join(preParseResult, "");

        // Stage 2: parse commands.
        std::cout << "Parsing command script. Stage 2: parsing commands..." << std::endl;
        PtslCmdFileScript result;
        doParseJsonScript(filteredStr, /*out*/ result);

        return result;
    }
    catch (const std::exception& e)
    {
        std::cout << "Input error: " << e.what() << std::endl;
    }

    return {};
}
