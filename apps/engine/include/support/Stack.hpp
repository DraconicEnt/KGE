/**
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
