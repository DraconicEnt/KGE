/**
 *  @file Tuple.hpp
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
