/**
 *  @file IncomingClient.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_GAME_OUTGOINGCLIENTSINGLETON_HPP_
#define _INCLUDE_GAME_OUTGOINGCLIENTSINGLETON_HPP_

#include <enet/enet.h>

#include <network/MessageBase.hpp>
#include <network/OutgoingClientBase.hpp>

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
        class OutgoingClientSingleton : public Kiaro::Network::OutgoingClientBase
        {
            // Public Methods
            public:
                /**
                 *  @brief Empty callback function for the the OnReceivePacket event.
                 *  @param packet The received packet.
                 */
                void onReceivePacket(Kiaro::Support::BitStream &incomingStream);

                void onConnected(void);

                void onDisconnected(void);

                void onConnectFailed(void);

                bool getIsOppositeEndian(void) { return mIsOppositeEndian; }

                void disconnect(void);

                Kiaro::Common::U16 getPort(void);

                static OutgoingClientBase *getPointer(void);
                static void destroy(void);

            // Private Methods
            private:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                OutgoingClientSingleton(ENetPeer *connecting, Kiaro::Network::ServerBase *server);

                ~OutgoingClientSingleton(void);

            // Private Members
            private:
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_OUTGOINGCLIENTSINGLETON_HPP_
