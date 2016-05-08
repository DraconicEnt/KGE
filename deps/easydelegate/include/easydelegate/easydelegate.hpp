/**
 *  @file easydelegate.hpp
 *  @date 7/11/2015
 *  @version 2.0
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
    #define EASYDELEGATE_NOEXCEPT noexcept
    #define EASYDELEGATE_CONSTEXPR constexpr
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
#ifdef EASYDELEGATE_FORCE_INLINE
    #define EASYDELEGATE_INLINE __forceinline
#else
    //! A preprocessor definition for a keyword that forces the inlining of a given method.
    #define EASYDELEGATE_INLINE
#endif

#include "types.hpp"
#include "delegates.hpp"
#include "delegateset.hpp"
#include "deferredcallers.hpp"

//! Namespace containing all EasyDelegate functionality.
namespace EasyDelegate
{

} // End Namespace EasyDelegate
#endif // _INCLUDE_EASYDELEGATE_HPP_
