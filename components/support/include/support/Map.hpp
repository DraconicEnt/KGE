/**
 *  @file Map.hpp
 */

#ifndef _INCLUDE_SUPPORT_MAP_HPP_
#define _INCLUDE_SUPPORT_MAP_HPP_

#include <map>

namespace Kiaro
{
    namespace Support
    {
		//! A typedef to an std::map.
        template <typename keyType, typename storedType>
        using Map = std::map<keyType, storedType>;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_MAP_HPP_
