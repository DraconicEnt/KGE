/**
 *  @file ServerBase.cpp
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

#include <network/IncomingClientBase.hpp>
#include <network/ServerBase.hpp>

namespace Kiaro
{
    namespace Network
    {
        ServerBase::ServerBase(const std::string &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount) : mIsRunning(true), mInternalHost(NULL),
        mListenPort(listenPort), mListenAddress(listenAddress)
        {
            ENetAddress enetAddress;
            enetAddress.port = listenPort;
            enet_address_set_host(&enetAddress, listenAddress.c_str());

            mInternalHost = enet_host_create(&enetAddress, maximumClientCount, 2, 0, 0);
            if (!mInternalHost)
            {
                mIsRunning = false;

                throw std::runtime_error("ServerBase: Failed to create ENet host!");
                   return;
            }
        }

        ServerBase::~ServerBase(void)
        {
            if (mInternalHost)
            {
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            mIsRunning = false;

            Kiaro::Support::MapDivision::Destroy();
        }

        void ServerBase::stop(void) { mIsRunning = false; }

        Kiaro::Common::U32 ServerBase::getClientCount(void)
        {
            return mConnectedClientSet.size();
        }

        void ServerBase::globalSend(Kiaro::Network::PacketBase *packet, const bool &reliable)
        {
            for (std::set<size_t>::iterator it = mConnectedClientSet.begin(); it != mConnectedClientSet.end(); it++)
            {
                Kiaro::Network::IncomingClientBase *currentRecipient = (Kiaro::Network::IncomingClientBase *)*it;
                currentRecipient->send(packet, reliable);
            }
        }

        void ServerBase::update(void)
        {
            if (!mIsRunning)
                return;

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        IncomingClientBase *client = new Kiaro::Network::IncomingClientBase(event.peer, this);
                        event.peer->data = client;

                        mConnectedClientSet.insert(mConnectedClientSet.end(), (size_t)client);
                        onClientConnected(client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        Kiaro::Network::IncomingClientBase *disconnected = (Kiaro::Network::IncomingClientBase*)event.peer->data;
                        onClientDisconnected(disconnected);

                        mConnectedClientSet.erase((size_t)disconnected);
                        delete disconnected;

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        if (!event.peer->data)
                        {
                            enet_packet_destroy(event.packet);
                            throw std::runtime_error("ServerBase: Invalid ENet peer data on packet receive!");
                        }

                        Kiaro::Network::IncomingClientBase *sender = (Kiaro::Network::IncomingClientBase*)event.peer->data;
                        Kiaro::Support::BitStream incomingStream(event.packet->data, event.packet->dataLength, event.packet->dataLength);

                        onReceivePacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
        }

        Kiaro::Network::IncomingClientBase *ServerBase::getLastPacketSender(void)
        {
            return NULL;
        }
    } // End Namespace Network
} // End Namespace Kiaro
