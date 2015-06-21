/**
 *  @file UnorderedMap.hpp
 */

#ifndef _INCLUDE_SUPPORT_UNORDEREDMAP_HPP_
#define _INCLUDE_SUPPORT_UNORDEREDMAP_HPP_

#include <unordered_map>

namespace Kiaro
{
    namespace Support
    {
		//! A typedef to an std::unordered_map.
        template <typename keyType, typename storedType>
        using UnorderedMap = std::unordered_map<keyType, storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_UNORDEREDMAP_HPP_
