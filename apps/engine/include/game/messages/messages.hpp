/**
 *  @file messages.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_HPP_

namespace Kiaro
{
    namespace Game
    {
        namespace Messages
        {
            enum MESSAGE_TYPE
            {
                MESSAGE_HANDSHAKE = 0x03,
                MESSAGE_SIMCOMMIT = 0x04,
                MESSAGE_DISCONNECT = 0x05,
            }; // End Enum MESSAGE_TYPE
        } // End NameSpace Packets
    } // End Namespace Game
} // End NameSpace Kiaro

#endif // _INCLUDE_KIARO_GAME_MESSAGES_HPP_
