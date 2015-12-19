 /**
 *  @file types.hpp
 *  @brief Include file declaring all the identifiers for known message types.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_NET_MESSAGES_IDENTIFIERS_HPP_
#define _INCLUDE_KIARO_NET_MESSAGES_IDENTIFIERS_HPP_

namespace Kiaro
{
    namespace Game
    {
        namespace Messages
        {
            enum TYPE
            {
                TYPE_HANDSHAKE = 0x03,
                TYPE_SIMCOMMIT = 0x04,
                TYPE_DISCONNECT = 0x05,
                TYPE_SCOPE = 0x06,
                TYPE_EXECUTERPC = 0x07,
            }; // End Enum MESSAGE_TYPE
        } // End NameSpace Packets
    } // End Namespace Game
} // End NameSpace Kiaro

#endif // _INCLUDE_KIARO_GAME_MESSAGES_HPP_

