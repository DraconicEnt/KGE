/**
 *  @file Tuple.hpp
 *  @brief Include file declaring the Support::Tuple type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_TUPLE_HPP_
#define _INCLUDE_SUPPORT_TUPLE_HPP_

#include <tuple>

namespace Kiaro
{
    namespace Support
    {
        //! A typedef to an std::tuple.
        template <typename... storedTypes>
        using Tuple = std::tuple<storedTypes...>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_TUPLE_HPP_
