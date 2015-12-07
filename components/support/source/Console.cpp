/**
 *  @file Console.cpp
 */

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Console
        {
            Support::UnorderedMap<Common::U8, Support::UnorderedSet<LogResponderPointer>> sLogResponders;
        }
    }
}

