/**
 *  @file Console.hpp
 *  @brief Include file declaring declaring console output helpers.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_CORE_LOGGING_HPP_
#define _INCLUDE_CORE_LOGGING_HPP_

#include <iostream>
#include <fstream>

#include <support/common.hpp>

#include <support/UnorderedSet.hpp>
#include <support/UnorderedMap.hpp>

#include <support/String.hpp>

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

            /**
             *  @brief A typedef for a pointer to a log responder method.
             *  @param type The message type that has been emitted.
             *  @param message The message string that has been emitted.
             */
            typedef void (*LogResponderPointer)(const MESSAGE_TYPE& type, const Support::String& message);

            extern Support::UnorderedMap<Common::U8, Support::UnorderedSet<LogResponderPointer>> sLogResponders;

            inline const Common::C8* messageTypeText(const MESSAGE_TYPE& type)
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

            template <typename... parameters>
            static void writef(const MESSAGE_TYPE& type, const Support::String& format, parameters... params)
            {
                // Call the responders first.
                Common::C8 buffer[256];
                sprintf(buffer, format.data(), params...);

                for (auto it = sLogResponders[type].begin(); it != sLogResponders[type].end(); it++)
                    (*it)(type, buffer);

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
             *  @brief Helper method to write a warning to the game console.
             *  @param format The string to format given parameters.
             */
            template <typename... parameters>
            static void warningf(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_WARNING, format.c_str(), params...);
            }

            /**
             *  @brief Helper method to write a error to the game console.
             *  @param format The string to format given parameters.
             */
            template <typename... parameters>
            static void errorf(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_ERROR, format.c_str(), params...);
            }
            
            template <typename... parameters>
            static void debugf(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_DEBUG, format.c_str(), params...);
            }

            /**
             *  @brief Helper method to write a info string to the game console.
             *  @param format The string to format given parameters.
             */
            template <typename... parameters>
            static void infof(const Support::String& format, parameters... params)
            {
                writef(MESSAGE_INFO, format.c_str(), params...);
            }

            static void write(const MESSAGE_TYPE& type, const Support::String& output)
            {
                // Call the responders first.
                puts(output.data());
            }

            static void info(const Support::String& output)
            {
                Console::write(MESSAGE_INFO, output);
            }

            static void error(const Support::String& output)
            {
                Console::write(MESSAGE_ERROR, output);
            }

            static void warning(const Support::String& output)
            {
                Console::write(MESSAGE_WARNING, output);
            }
            
            static void debug(const Support::String& output)
            {
                Console::write(MESSAGE_DEBUG, output);
            }
            
            #define QUOTE_IMPL(T) #T
            #define QUOTE(T) QUOTE_IMPL(T)

            #define ASSEMBLE_OUTPUT(format) __FILE__:__LINE__:: format
            #define CONSOLE_ERROR(format, ...) Support::Console::errorf(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
            #define CONSOLE_INFO(format, ...) Support::Console::infof(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
            #define CONSOLE_WARNING(format, ...) Support::Console::warningf(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
            #define CONSOLE_DEBUG(format, ...) Support::Console::debugf(QUOTE(ASSEMBLE_OUTPUT(format)), ##__VA_ARGS__)
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_LOGGING_HPP_
