/**
 *  @file Common.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_COMMON_
#define _INCLUDE_KIARO_ENGINE_COMMON_

#include <sstream>
#include <cassert>
#include <iostream>

#include <math.h>
#include <string.h>
#include <stdio.h>

#include <functional> // std::hash

#include <irrlicht.h>

//#include <boost/unordered_map.hpp>

// Temporary Windows build work around
#define _WINSOCKAPI_

namespace Kiaro
{
    //! An enumeration used to store versioning information.
    enum VERSION
    {
        MAJOR = 0,
        MINOR = 0,
        BUILD = 0,
        REVISION = 0,
        PROTOCOL = 0
    };

    //! Enumeration used to represent all possible engine exit statuses.
    enum EXIT_STATUS_NAME
    {
        //! Good exit.
        EXIT_STATUS_GOOD = 0,
        //! Input error.
        EXIT_STATUS_INPUT = 1,
        //! Memory allocation error.
        EXIT_STATUS_MEMALLOC = 2,
        //! Engine exited via Kill()
        EXIT_STATUS_KILLED = 3,
    };

    namespace Common
    {
#define _ENGINE_USE_IRRLICHT_TYPES_ 1
#define _ENGINE_USE_GLOBAL_EXCEPTION_CATCH_ 0

#define ENGINE_ASSERT_TRUE(x) assert(x)

#if defined(_MSC_VER) || (__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__)
        //#define NOTHROW __declspec(nothrow)
#define NOTHROW
#else
#define NOTHROW __attribute__((nothrow))
#endif

#define NOEXCEPT
#define CONSTEXPR

#ifndef _ENGINE_USE_IRRLICHT_TYPES_
#if defined(_MSC_VER) || (__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__)
        typedef signed __int8 S8;
        typedef unsigned __int8 U8;
        typedef unsigned __int16 U16;
        typedef signed __int16 S16;
        typedef signed __int32 S32;
        typedef unsigned __int32 U32;
#else
        typedef signed char S8;
        typedef unsigned char U8;
        typedef unsigned short U16;
        typedef signed short S16;
        typedef signed int S32;
        typedef unsigned int U32;
#endif

        //! 32-bit Signed Float typedef.
        typedef float F32;
        //! 64-bit Signed Float typedef.
        typedef double F64;
        //! Signed char typedef.
        typedef char C8;
        //! 64-bit integer typedef.
        typedef unsigned long long int U64;
#else
        typedef irr::s8 S8;
        typedef irr::u8 U8;
        typedef irr::u16 U16;
        typedef irr::s16 S16;
        typedef irr::s32 S32;
        typedef irr::u32 U32;

        typedef irr::f32 F32;
        typedef irr::f64 F64;
        typedef irr::c8 C8;
        typedef irr::u64 U64;

        typedef irr::core::vector3df Vector3DF;
        typedef irr::video::SColor ColorRGBA;
#endif
    } // End NameSpace Common
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_COMMON_
