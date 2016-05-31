/**
 *  @file Deque.hpp
 *  @brief Include file declaring the Support::Deque type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_DEQUE_HPP_
#define _INCLUDE_SUPPORT_DEQUE_HPP_

#include <deque>

namespace Kiaro
{
    namespace Support
    {
        //! A typedef to an std::map.
        template <typename storedType>
        using Deque = std::deque<storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_DEQUE_HPP_

