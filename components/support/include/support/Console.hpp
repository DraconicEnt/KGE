/**
 *  @file Console.hpp
 *  @brief Include file declaring declaring console output helpers.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CORE_LOGGING_HPP_
#define _INCLUDE_CORE_LOGGING_HPP_

#include <iostream>
#include <fstream>

#include <support/common.hpp>

#include <support/UnorderedSet.hpp>
#include <support/UnorderedMap.hpp>

#include <support/String.hpp>

#include <easydelegate/easydelegate.hpp>

namespace Kiaro
{
    namespace Support
    {
        //! A namespace containing all of the console output functionality of the engine.
        namespace Console
        {
            //! An enumeration representing the possible console message types.
            enum MESSAGE_TYPE
            {
                //! No message type.
                MESSAGE_NULL = 0,
                //! Informational.
                MESSAGE_INFO = 1,
                //! A warning.
                MESSAGE_WARNING = 2,
                //! Some non-fatal error.
                MESSAGE_ERROR = 3,
                //! The worst possible error, the engine should terminate following printing this.
                MESSAGE_FATAL = 4,
                //! Debug-only messages.
                MESSAGE_DEBUG = 5,
            };

            typedef EasyDelegate::DelegateSet<void, MESSAGE_TYPE, const Support::String&> LogResponderSetType;

            /**
             *  @brief A typedef for a pointer to a log responder method.
             *  @param type The message type that has been emitted.
             *  @param message The message string that has been emitted.
             */
            typedef void (*LogResponderPointer)(MESSAGE_TYPE type, const Support::String& message);

            extern Support::UnorderedMap<Common::U8, LogResponderSetType> sLogResponders;

            inline const Common::C8* messageTypeText(MESSAGE_TYPE type)
            {
                switch(type)
                {
                    case MESSAGE_DEBUG:
                        return "DEBUG";

                    case MESSAGE_ERROR:
                        return "ERROR";

                    case MESSAGE_FATAL:
                        return "FATAL";

                    case MESSAGE_INFO:
                        return "INFO";

                    case MESSAGE_WARNING:
                        return "WARNING";
                }

                return "UNKNOWN";
            }

            static void registerListener(LogResponderSetType::StaticDelegateFuncPtr method, MESSAGE_TYPE type)
            {
                sLogResponders[type].push_back(new LogResponderSetType::StaticDelegateType(method));
            }

            template <typename className>
            static void registerListener(LogResponderSetType::MemberDelegateFuncPtr<className> method, className* thisPtr, MESSAGE_TYPE type)
            {
                sLogResponders[type].push_back(new LogResponderSetType::MemberDelegateType<className>(method, thisPtr));
            }

            /**
             *  @brief Writes a formatted message to the console as the given message type.
             *  @param type The message type to write as.
             *  @param format The string to format given the varible length parameter list.
             *  @param params The variable length parameter list to format the format string with.
             */
            template <typename... parameters>
            static void writef(MESSAGE_TYPE type, const Support::String& format, parameters... params)
            {
                // Call the responders first.
                Common::C8 buffer[256];
                sprintf(buffer, format.data(), params...);
                sLogResponders[type].invoke(type, buffer);
                printf("(%s) ", messageTypeText(type));

                if (sizeof...(params) >= 1)
                    printf(format.data(), params...);
                else
                {
                    puts(format.data());
                    return;
                }

                puts("");
            }

            /**
             *  @brief Helper method to write a warning message to the game console.
             *  @param format The string to format given the varible length parameter list.
             *  @param params The variable length parameter list to format the format string with.
             */
            template <typename... parameters>
            static void warningf(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_WARNING, format.c_str(), params...);
            }

            /**
             *  @brief Helper method to write an error message to the game console.
             *  @param format The string to format given the varible length parameter list.
             *  @param params The variable length parameter list to format the format string with.
             */
            template <typename... parameters>
            static void errorf(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_ERROR, format.c_str(), params...);
            }

            /**
             *  @brief Helper method to write a debug message to the game console.
             *  @param format The string to format given the varible length parameter list.
             *  @param params The variable length parameter list to format the format string with.
             */
            template <typename... parameters>
            static void debugf(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_DEBUG, format.c_str(), params...);
            }

            /**
             *  @brief Helper method to write an info string to the game console.
             *  @param format The string to format given the varible length parameter list.
             *  @param params The variable length parameter list to format the format string with.
             */
            template <typename... parameters>
            static void infof(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_INFO, format.c_str(), params...);
            }

            /**
             *  @brief Helper method to test for an assertion and write formatted messages to the console.
             *  @param format The string to format given the varible length parameter list.
             */
            template <typename... parameters>
            static void assertf(bool expressionValue, const Support::String& format, parameters... params)
            {
                if (!expressionValue)
                    errorf(format, params...);
            }

            /**
             *  @brief Writes an unformatted message to the console as the given message type.
             *  @param type The message type to write as.
             *  @param output The string to write to the console.
             */
            static void write(MESSAGE_TYPE type, const Support::String& output)
            {
                sLogResponders[type].invoke(type, output);
                printf("(%s) %s", messageTypeText(type), output.data());
                puts("");
            }

            /**
             *  @brief Helper method to write an unformatted info message to the console.
             *  @param output The string to write to the console.
             */
            static void info(const Support::String& output)
            {
                write(MESSAGE_INFO, output);
            }

            /**
             *  @brief Helper method to write an unformatted error message to the console.
             *  @param output The string to write to the console.
             */
            static void error(const Support::String& output)
            {
                write(MESSAGE_ERROR, output);
            }

            /**
             *  @brief Helper method to write an unformatted warning message to the console.
             *  @param output The string to write to the console.
             */
            static void warning(const Support::String& output)
            {
                write(MESSAGE_WARNING, output);
            }

            /**
             *  @brief Helper method to write an unformatted debug message to the console.
             *  @param output The string to write to the console.
             */
            static void debug(const Support::String& output)
            {
                write(MESSAGE_DEBUG, output);
            }


            #define QUOTE_IMPL(T) #T
            #define QUOTE(T) QUOTE_IMPL(T)

            #define ASSEMBLE_OUTPUT(format) __FILE__:__LINE__:: format
            #define ASSEMBLE_ASSERTION(expression, format) Failed assertion (expression) at __FILE__:__LINE__:: format

            //! Helper define to perform an assertion that the engine handles.
            #define CONSOLE_ASSERT(expression, format, ...) Support::Console::assertf(expression, QUOTE(ASSEMBLE_ASSERTION(#expression, format)), ##__VA_ARGS__); assert(expression)
            //! Helper define to emit a formatted console error message.
            #define CONSOLE_ERRORF(format, ...) Support::Console::errorf(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
            //! Helper define to emit a formatted console info message.
            #define CONSOLE_INFOF(format, ...) Support::Console::infof(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
            //! Helper define to emit a formatted console warning message.
            #define CONSOLE_WARNINGF(format, ...) Support::Console::warningf(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
            //! Helper define to emit a formatted console debug message.
            #define CONSOLE_DEBUGF(format, ...) Support::Console::debugf(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)

            //! Helper define to emit a regular console error message.
            #define CONSOLE_ERROR(message, ...) Support::Console::error(QUOTE(ASSEMBLE_OUTPUT(message)))
            //! Helper define to emit a regular console info message.
            #define CONSOLE_INFO(message, ...) Support::Console::info(QUOTE(ASSEMBLE_OUTPUT(message)))
            //! Helper define to emit a regular console warning message.
            #define CONSOLE_WARNING(message, ...) Support::Console::warning(QUOTE(ASSEMBLE_OUTPUT(message)))
            //! Helper define to emit a regular console debug message.
            #define CONSOLE_DEBUG(message, ...) Support::Console::debug(QUOTE(ASSEMBLE_OUTPUT(message)))
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_LOGGING_HPP_
