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

            typedef void (*LogResponder)(const MESSAGE_TYPE& type, const Support::String& message);

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

            static void write(const MESSAGE_TYPE& type, const Support::String& output)
            {
                // Call the responders first.
                puts(output.data());
            }
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_LOGGING_HPP_
