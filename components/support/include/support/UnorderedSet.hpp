/**
 *  @file UnorderedSet.hpp
 *  @brief Include file declaring the Support::UnorderedSet type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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
