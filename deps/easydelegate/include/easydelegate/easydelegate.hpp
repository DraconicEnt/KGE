/**
 *  @file easydelegate.hpp
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Main include file for the EasyDelegate library.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt for more
 *	information.
 */

#ifndef _INCLUDE_EASYDELEGATE_HPP_
#define _INCLUDE_EASYDELEGATE_HPP_

// Define __forceinline if we're on GCC
#if defined(__GNUC__) || defined(__GNUG__)
    #define __forceinline __attribute__((always_inline))

    // These keywords don't mean anything if we're compiling with an old standard
    #if __cplusplus >= 201103L
        #define EASYDELEGATE_NOEXCEPT noexcept
        #define EASYDELEGATE_CONSTEXPR constexpr
    #else
        #define EASYDELEGATE_NOEXCEPT
        #define EASYDELEGATE_CONSTEXPR
    #endif
// VS15 and onwards should support these
#elif _MSC_VER >= 1900
	#define EASYDELEGATE_NOEXCEPT noexcept
	#define EASYDELEGATE_CONSTEXPR constexpr
// Assume we don't support noexcept and constexpr
#else
    //! A macro that is used to noexcept to something that is compatible with the compiler.
	#define EASYDELEGATE_NOEXCEPT
	//! A macro that is used to declare constexpr to something that is compatible with the compiler.
	#define EASYDELEGATE_CONSTEXPR
#endif

// If we're going to inline stuff, force it
#if defined(EASYDELEGATE_FORCE_INLINE) && __cplusplus >= 201103L
    #define EASYDELEGATE_INLINE __forceinline
#else
    //! A preprocessor definition for a keyword that forces the inlining of a given method.
    #define EASYDELEGATE_INLINE
#endif

#include "types.hpp"

#if __cplusplus >= 201103L
#include "delegates.hpp"
#include "delegateset.hpp"
#include "deferredcallers.hpp"
#else
#include "delegatesCompat.hpp"
#include "delegatesetCompat.hpp"
#endif

//! Namespace containing all EasyDelegate functionality.
namespace EasyDelegate
{

} // End Namespace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_HPP_
