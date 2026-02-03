// Copyright 2023-2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation of the ptslcmd file script parser.
 */

#include <iostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <nlohmann/json.hpp>

#include "FileScriptParser.h"

using boost::fusion::operator<<;
using json = nlohmann::json;

BOOST_FUSION_ADAPT_STRUCT(FileScript::Command, Name, Arguments)
BOOST_FUSION_ADAPT_STRUCT(FileScript, Commands)

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

        // Filter out all thise types of comments
        auto cppcomment = x3::omit[x3::lit("//") >> *(x3::char_ - (x3::eol | x3::eoi))];
        auto pycomment = x3::omit[x3::lit("#") >> *(x3::char_ - (x3::eol | x3::eoi))];
        auto ccomment = x3::omit[x3::lit("/*") > *(x3::char_ - x3::lit("*/")) > x3::lit("*/")];
        // Match quoted string to keep "comments" inside them.
        auto qstring_simple = x3::lexeme[x3::char_('"') > *(x3::string("\\\"") | ~x3::char_('"')) > x3::char_('"')];

        const x3::rule<struct comments_filter, std::vector<std::string>> comments_filter = "comments_filter";
        auto comments_filter_def = *(qstring_simple | cppcomment | ccomment | pycomment
            | +(char_ - (x3::lit('"') | x3::lit("//") | x3::lit("#") | x3::lit("/*"))));

        BOOST_SPIRIT_DEFINE(comments_filter);
    } // namespace parser
} // namespace comments

namespace commands
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

        struct ErrorHandler
        {
            template <typename Iterator, typename Exception, typename Context>
            x3::error_handler_result on_error(
                Iterator& first, const Iterator& last, const Exception& x, const Context& context)
            {
                auto& handler = x3::get<x3::error_handler_tag>(context).get();
                std::string message = "Input error. Expecting: " + x.which() + " here:";
                handler(x.where(), message);
                return x3::error_handler_result::fail;
            }
        };

        // Match quoted string unpacking it from quotes and updating escaped symbols.
        auto qstring = x3::lexeme['"'
            > *("\\n" >> x3::attr('\n') | "\\b" >> x3::attr('\b') | "\\f" >> x3::attr('\f') | "\\t" >> x3::attr('\t')
                | "\\v" >> x3::attr('\v') | "\\r" >> x3::attr('\r') | "\\n" >> x3::attr('\n')
                | "\\" >> x3::char_("\"\\") | "\\" >> x3::int_parser<char, 8, 1, 3>()
                | "\\x" >> x3::int_parser<char, 16, 2, 2>() | ~x3::char_('"'))
            > '"'];

        auto spaceWithoutEol = x3::space - x3::eol;

        const x3::rule<struct command_argument, std::string> command_argument = "command_argument";
        const x3::rule<struct command, FileScript::Command> command = "command";
        const x3::rule<struct script, std::vector<FileScript::Command>> script = "script";

        const auto command_argument_def = (qstring | +ascii::graph);
        const auto command_def = (+lexeme[(ascii::alnum | ascii::char_("_"))]
                                     >> *(x3::omit[*spaceWithoutEol] >> command_argument)
                                     >> x3::omit[*(spaceWithoutEol)])
            > (x3::eol | x3::eoi);
        const auto script_def = x3::omit[*x3::space] >> +(command >> x3::omit[*x3::space]) > x3::eoi;

        BOOST_SPIRIT_DEFINE(command_argument, command, script);

        struct command_argument
        {
        };

        struct command : x3::annotate_on_success
        {
        };

        struct script : ErrorHandler, x3::annotate_on_success
        {
        };

    } // namespace parser
} // namespace commands

std::optional<FileScript> FileScriptParser::Parse(const std::string& text, bool isJsonFile)
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

        // When parsing a JSON file, we iterate through each command, passing `json_request` in the args as a string,
        // as though it were passed from the command line. This approach automatically supports `-json_request` arg
        // in the command line interface.
        auto doParseJson = [](const auto& str, auto& result)
        {
            json jObject = json::parse(str);

            const std::string commandsTag = "commands";
            const std::string commandNameTag = "command_name";
            const std::string commandRequestBodyTag = "json_request";
            const std::string commandDirectRequestTag = "is_direct_request";
            const std::string commandWaitForResponseTag = "post_launch_delay_ms";

            // TODO: (ashevche, 24.01.2024) We need to extract a common header file which would contain these constants:
            const std::string jsonRequestArgName = "-json_request"; // whether the request is in a JSON form;
                                                                    // soon, it'll become the only option
                                                                    // (once we get rid of CLI command args parsing)

            const std::string isJsonRequestDirectArgName =
                "-is_json_request_direct"; // whether the JSON request should be
                                           // passed directly to gRPC client

            int currCmdNo = 0;
            for (const auto& command : jObject[commandsTag])
            {
                ++currCmdNo;

                if (!command.contains(commandNameTag))
                {
                    throw std::runtime_error(
                        "In the JSON script, command #" + std::to_string(currCmdNo) + " lacks a specified name");
                }

                FileScript::Command scriptCmd;
                scriptCmd.Name = command[commandNameTag];

                if (command.contains(commandRequestBodyTag))
                {
                    json jBody = command[commandRequestBodyTag];
                    scriptCmd.Arguments.push_back(jsonRequestArgName);
                    scriptCmd.Arguments.push_back(jBody.dump());
                }

                if (command.contains(commandDirectRequestTag))
                {
                    bool isDirectRequest = command[commandDirectRequestTag];
                    // Currently, all JSON requests are direct
                    // TODO: (ashevche, 29.01.2024) when ready, support the option
                    // if (isDirectRequest) scriptCmd.Arguments.push_back(isJsonRequestDirectArgName);
                }

                if (command.contains(commandWaitForResponseTag))
                {
                    scriptCmd.PostLaunchDelayMs = command[commandWaitForResponseTag];
                }

                result.Commands.push_back(scriptCmd);
            }
        };

        // Stage 1: remove the comments and regenerate a new input string.
        std::cout << "Parsing stage 1..." << std::endl;
        std::vector<std::string> preParseResult;
        doParse(text, comments::parser::comments_filter, /*out*/ preParseResult);

        auto filteredStr = boost::algorithm::join(preParseResult, "");

        // Stage 2: parse commands.
        std::cout << "Parsing stage 2..." << std::endl;
        FileScript result;

        if (isJsonFile)
        {
            doParseJson(filteredStr, /*out*/ result);
        }
        else
        {
            doParse(filteredStr, commands::parser::script, /*out*/ result);
        }

        return result;
    }
    catch (const std::exception& e)
    {
        std::cout << "Input error: " << e.what() << std::endl;
    }

    return {};
}
