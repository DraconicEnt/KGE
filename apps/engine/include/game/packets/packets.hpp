/**
 *  @file packets.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_PACKETS_HPP_
#define _INCLUDE_KIARO_GAME_PACKETS_HPP_

namespace Kiaro
{
    namespace Game
    {
        namespace Packets
        {
            enum PACKET_TYPE
            {
                PACKET_HANDSHAKE = 0x03,
            }; // End Enum PACKET_TYPE
        } // End NameSpace Packets
    } // End Namespace Game
} // End NameSpace Kiaro

#include <game/packets/HandShake.hpp>

#endif // _INCLUDE_KIARO_GAME_PACKETS_HANDSHAKE_HPP_
