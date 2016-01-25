/**
 *  @file Stack.hpp
 *  @brief Include file declaring the Support::Stack type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_STACK_HPP_
#define _INCLUDE_SUPPORT_STACK_HPP_

#include <stack>

namespace Kiaro
{
    namespace Support
    {
        template <typename storedType>
        using Stack = std::stack<storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_STACK_HPP_
