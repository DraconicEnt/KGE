/**
 *  @file Packet.cpp
 *  @brief Source code for Packet instances.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#include <enet/enet.h>

#include <network/Packet.h>

#include <stdio.h>
#include <string.h>

namespace Kiaro
{
    namespace Network
    {
        Packet::Packet(ENetPacket *ReceivedPacket) : ReceivedSize(ReceivedPacket->dataLength)
        {
            this->ReceivedPacket = ReceivedPacket;
            this->ReceivedData = ReceivedPacket->data;
        }

        Packet::~Packet(void)
        {
            if (this->ReceivedPacket != 0x00)
                enet_packet_destroy(this->ReceivedPacket);

            this->ReceivedPacket = 0x00;
        }
    } // End NameSpace Network
} // End NameSpace Kiaro
