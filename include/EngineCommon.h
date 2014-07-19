/**
 *  @file Common.h
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_COMMON_
#define _INCLUDE_KIARO_COMMON_

#include <math.h>
#include <string.h>
#include <stdio.h>

#include <boost/unordered_map.hpp>

namespace Kiaro
{
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

        //! A global hasher.
        static boost::hash<std::string> string_hash;

        //! An assertion used to verify that floats are of the expected sizes.
        static_assert(sizeof(F32) == 4, "Floating point numbers are not 4-byte.");
        static_assert(sizeof(F64) == 8, "Double precision numbers are not 8-byte.");
    } // End NameSpace Common
} // End NameSpace Kiaro

#endif // _INCLUDE_KIARO_COMMON_
