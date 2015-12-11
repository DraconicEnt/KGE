/**
 *  @file String.hpp
 *  @brief Include file declaring the Support::String type and associated types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_STRING_HPP_
#define _INCLUDE_SUPPORT_STRING_HPP_

#include <string>

namespace Kiaro
{
    namespace Support
    {
        typedef std::string String;
        
        static std::hash<std::string> getHashCode;
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_STRING_HPP_
