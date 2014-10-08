/**
 *  @file packets.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
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
