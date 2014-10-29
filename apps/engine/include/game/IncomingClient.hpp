/**
 *  @file IncomingClient.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_GAME_INCOMINGCLIENT_HPP_
#define _INCLUDE_GAME_INCOMINGCLIENT_HPP_

#include <network/IncomingClientBase.hpp>

namespace Kiaro
{
    namespace Support
    {
        class BitStream;
    } // End NameSpace Support

    namespace Network
    {
        class ServerBase;
        class PacketBase;
    } // End NameSpace Network

    namespace Game
    {
        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class IncomingClient : public Kiaro::Network::IncomingClientBase
        {
            // Public Methods
            public:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                IncomingClient(ENetPeer *connecting, Kiaro::Network::ServerBase *server);

                ~IncomingClient(void);

                /**
                 *  @brief Empty callback function for the the OnReceivePacket event.
                 *  @param packet The received packet.
                 */
                void OnReceivePacket(Kiaro::Support::BitStream &incomingStream);

                void Send(Kiaro::Network::PacketBase *packet, const bool &reliable);

                bool GetIsOppositeEndian(void) { return mIsOppositeEndian; }

                void Disconnect(void);

                Kiaro::Common::U16 GetPort(void);

            private:
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_INCOMINGCLIENT_HPP_
