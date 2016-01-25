/**
 *  @file UnorderedMap.hpp
 *  @brief Include file declaring the Support::UnorderedMap type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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
