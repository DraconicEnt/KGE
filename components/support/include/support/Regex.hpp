/**
 *  @file Regex.hpp
 *  @brief Include file declaring various regular expression types and methods. It also
 *  resolves at compile-time whether or not to use Boost's regex functionality or those provided
 *  by the STL since C++11.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_REGEX_HPP_
#define _INCLUDE_SUPPORT_REGEX_HPP_

#include <support/String.hpp>

#ifdef ENGINE_USE_BOOST_REGEX
    #include <boost/regex.hpp>
#else
    #include <regex>
#endif

namespace Kiaro
{
    namespace Support
    {
        #ifdef ENGINE_USE_BOOST_REGEX
            typedef boost::regex Regex;
            typedef boost::wregex WRegex;
        #else
            // Regex wasn't implemented until GCC 4.9
            #if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__  == 4 && __GNUC_MINOR__ < 9))
                #error std::regex is broken in some versions of GCC prior to 4.9! Please use the boost::regex support or upgrade your compiler.
            #endif

            typedef std::regex Regex;
            typedef std::wregex WRegex;
        #endif

        bool RegexMatch(const Support::String& input, const Support::Regex& pattern);

		namespace RegexConstants
		{
			#ifdef ENGINE_USE_BOOST_REGEX
			static const auto Basic = boost::regex_constants::basic;
			static const auto Extended = boost::regex_constants::extended;
			#else
			static const auto Basic = std::regex_constants::basic;
			static const auto Extended = std::regex_constants::extended;
			#endif
		}
    }
}
#endif // _INCLUDE_SUPPORT_REGEX_HPP_
