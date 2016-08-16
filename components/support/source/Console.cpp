/**
 *  @file Console.cpp
 *  @brief Source file implementing console subsystems.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Console
        {
            Support::UnorderedMap<Common::U8, LogResponderSetType> sLogResponders;
        }
    }
}

