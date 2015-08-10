/**
 *  @file Set.hpp
 */

#ifndef _INCLUDE_SUPPORT_SET_HPP_
#define _INCLUDE_SUPPORT_SET_HPP_

#include <set>

namespace Kiaro
{
    namespace Support
    {
		//! A typedef to an std::set.
        template <typename storedType>
        using Set = std::set<storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_SET_HPP_
