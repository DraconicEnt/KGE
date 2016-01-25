/**
 *  @file Vector.hpp
 *  @brief Include file declaring the Support::Vector type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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
