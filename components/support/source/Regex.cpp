#include <support/Regex.hpp>

namespace Kiaro
{
    namespace Support
    {
        bool RegexMatch(const Support::String& input, const Support::Regex& pattern)
        {
#ifdef ENGINE_USE_BOOST_REGEX
            return boost::regex_match(input, pattern);
#else
            return std::regex_match(input, pattern);
#endif
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
