/**
 *  @file OutgoingClientSingleton.cpp
 *  @brief Source code file defining logic for the Kiaro::Network::OutgoingClient class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 3/5/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <enet/enet.h>
#include <boost/regex.hpp>

#include <engine/Logging.hpp>
#include <engine/CoreSingleton.hpp>

#include <game/packets/packets.hpp>
#include <game/OutgoingClientSingleton.hpp>

#include <support/BitStream.hpp>

#include <network/OutgoingClientBase.hpp>

namespace Kiaro
{
    namespace Game
    {
        Kiaro::Network::OutgoingClientBase *OutgoingClientSingleton_Instance = NULL;

        OutgoingClientSingleton::OutgoingClientSingleton(ENetPeer *incoming, Kiaro::Network::ServerBase *server)
        {

        }

        OutgoingClientSingleton::~OutgoingClientSingleton(void)
        {
            if (mInternalPeer)
            {
                enet_peer_reset(mInternalPeer);
                mInternalPeer = NULL;
            }

            if (mInternalHost)
            {
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }
        }

        void OutgoingClientSingleton::onReceivePacket(Kiaro::Support::BitStream &incomingStream)
        {
            // We need to know what type of packet it is first
            Kiaro::Network::PacketBase basePacket;
            basePacket.unpackData(incomingStream);

            switch (basePacket.getType())
            {
                case Kiaro::Game::Packets::PACKET_HANDSHAKE:
                {
                    Kiaro::Game::Packets::HandShake receivedHandshake;
                    receivedHandshake.unpackData(incomingStream);

                    // NOTE: Would rather printf here but then the stdout override doesn't work
                    std::cout << "OutgoingClient: Server Version is " << (Kiaro::Common::U32)receivedHandshake.mVersionMajor << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionMinor << "." << (Kiaro::Common::U32)receivedHandshake.mVersionRevision << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionBuild << std::endl;

                    mCurrentStage = 1;

                    break;
                }
            }
        }

        void OutgoingClientSingleton::onConnected(void)
        {
            std::cout << "OutgoingClient: Established connection to remote host" << std::endl;

            // Dispatch our own handshake in response
            Kiaro::Game::Packets::HandShake handShake;
            handShake.mVersionMajor = 1;
            handShake.mVersionMinor = 2;
            handShake.mVersionRevision = 3;
            handShake.mVersionBuild = 4;

            send(&handShake, true);
        }

        void OutgoingClientSingleton::onDisconnected(void)
        {
            std::cout << "OutgoingClient: Received disconnect event" << std::endl;
        }

        void OutgoingClientSingleton::onConnectFailed(void)
        {

        }

        Kiaro::Network::OutgoingClientBase *OutgoingClientSingleton::getPointer(void)
        {
            if (!OutgoingClientSingleton_Instance)
                OutgoingClientSingleton_Instance = new OutgoingClientSingleton(NULL, NULL);

            return OutgoingClientSingleton_Instance;
        }

        void OutgoingClientSingleton::destroy(void)
        {
            if (OutgoingClientSingleton_Instance)
            {
                delete OutgoingClientSingleton_Instance;
                OutgoingClientSingleton_Instance = NULL;
            }
        }
    } // End Namespace Network
} // End Namespace Kiaro
