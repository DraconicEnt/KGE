/**
 *  @file Set.hpp
 *  @brief Include file declaring the Support::Set type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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
