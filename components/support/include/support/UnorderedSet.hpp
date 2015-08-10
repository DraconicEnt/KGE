/**
 *  @file UnorderedSet.hpp
 */

#ifndef _INCLUDE_SUPPORT_UNORDEREDSET_HPP_
#define _INCLUDE_SUPPORT_UNORDEREDSET_HPP_

#include <unordered_set>

namespace Kiaro
{
    namespace Support
    {
        //! A typedef to an std::unordered_set.
        template <typename storedType>
        using UnorderedSet = std::unordered_set<storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_UNORDEREDSET_HPP_
