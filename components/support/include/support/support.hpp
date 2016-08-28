/**
 *  @file support.cpp
 *  @brief Include file including all the engine support implementations.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_SUPPORT_HPP_
#define _INCLUDE_KIARO_SUPPORT_HPP_

#include <support/CBitStream.hpp>
#include <support/CCommandLineParser.hpp>
#include <support/CFileBitStream.hpp>
#include <support/CMapDivision.hpp>
#include <support/FEndian.hpp>
#include <support/FTime.hpp>

#include <support/Regex.hpp>

#include <support/Math.hpp>
#include <support/Tuple.hpp>
#include <support/UnorderedMap.hpp>
#include <support/Vector.hpp>
#include <support/Set.hpp>
#include <support/UnorderedSet.hpp>
#include <support/Map.hpp>

#include <support/types.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief Helper method to throw an exception with a formatted message.
         *  @param exceptionType The exception type to throw.
         *  @param parameters All parameters to be used in the formatting using sprintf formatting rules.
         *  @param format The format string using sprintf formatting rules.
         *  @param params All parameters to be used in the formatting using sprintf formatting rules.
         */
        template <typename exceptionType, typename... parameters>
        constexpr static void throwFormattedException(const Support::String& format, parameters... params)
        {
            Common::C8 outputBuffer[256];
            sprintf(outputBuffer, format.data(), params...);

            throw exceptionType(outputBuffer);
        }

        /**
         *  @brief Helper method to throw an exception.
         *  @param exceptionType The exception type to throw.
         *  @param message The message to throw in the exception.
         */
        template <typename exceptionType>
        constexpr static void throwException(const Support::String& message)
        {
            throw exceptionType(message.data());
        }
    } // End NameSpace Support
} // End NameSpace Kiaro

// Ensure that we're building against an SDL-enabled Irrlicht
#if !defined(_IRR_COMPILE_WITH_SDL_DEVICE_) || defined(NO_IRR_COMPILE_WITH_SDL_DEVICE_)
    #error The engine must be built against an SDL device enabled Irrlicht build.
#endif

#endif // _INCLUDE_KIARO_SUPPORT_HPP_
