/**
 *  @file Packet.h
 *  @brief Include file defining the Packet class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_NETWORK_PACKET_H_
#define _INCLUDE_KIARO_NETWORK_PACKET_H_

#include "EngineCommon.h"

namespace Kiaro
{
    namespace Network
    {
        // NOTE (Robert MacGregor#9): Hacky Ass Forward Declaration..
        class IncomingClient;

        //! Class representing a packet that has or is ready to traverse across the network to a remote host.
        class Packet
        {
            public:
            /**
             *  @brief Constructor that accepts a received netpacket from the underlaying networking subsystem.
             *  @param received A packet from the internal networking subsystem to construct the class from.
             */
            Packet(ENetPacket *received);
            //! Destroys the instance and the underlaying subsystem's packet data.
            ~Packet(void);

            //! A pointer to a Client instance that sent the data. If this is NULL, then the origin was a Server.
            Kiaro::Network::IncomingClient *Sender;

            //! The actual contained in this packet.
            void *ReceivedData;

            //! The size of the data in bytes.
            const Kiaro::Common::U32 ReceivedSize;

            private:
            //! Internal packet that was received from the networking subsystem.
            ENetPacket *ReceivedPacket;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_NETWORK_PACKET_H_
