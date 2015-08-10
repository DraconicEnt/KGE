/**
 *  @file Vector.hpp
 */

#ifndef _INCLUDE_SUPPORT_VECTOR_HPP_
#define _INCLUDE_SUPPORT_VECTOR_HPP_

#include <vector>

namespace Kiaro
{
    namespace Support
    {
        //! A typedef to an std::vector.
        template <typename storedType>
        using Vector = std::vector<storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_VECTOR_HPP_
