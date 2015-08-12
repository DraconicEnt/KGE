/**
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
        namespace Logging
        {
            enum MESSAGE_TYPE
            {
                MESSAGE_NULL = 0,
                MESSAGE_INFO = 1,
                MESSAGE_WARNING = 2,
                MESSAGE_ERROR = 3,
                MESSAGE_FATAL = 4,
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
            static void write(const MESSAGE_TYPE& type, const Support::String& format, parameters... params)
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
        }
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_LOGGING_HPP_
