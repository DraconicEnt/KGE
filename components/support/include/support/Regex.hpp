
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

        enum RegexConstants
        {
            #ifdef ENGINE_USE_BOOST_REGEX
                Basic = boost::regex_constants::basic,
                Extended = boost::regex_constants::extended,
            #else
                Basic = std::regex_constants::basic,
                Extended = std::regex_constants::extended,
            #endif
        };
    }
}
#endif // _INCLUDE_SUPPORT_REGEX_HPP_
