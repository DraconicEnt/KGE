/**
 *  @file OutgoingClient.cpp
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

#include <boost/regex.hpp>

#include <enet/enet.h>

#include <EngineLogging.h>
#include <network/OutgoingClient.h>
//#include <network/Common.h>
//#include <network/RemoteClient.h>

namespace Kiaro
{
    namespace Network
    {
        Kiaro::Network::OutgoingClient *OutgoingClient_Singleton = 0x00;

        OutgoingClient::OutgoingClient(void) : Connected(false), Port(0x00), Peer(0x00), Host(0x00)
        {

        }

        OutgoingClient::~OutgoingClient(void)
        {
            if (this->Peer)
            {
                enet_peer_reset(this->Peer);
                this->Peer = 0x00;
            }

            if (this->Host)
            {
                enet_host_destroy(this->Host);
                this->Host = 0x00;
            }
        }

        void OutgoingClient::Send(const Kiaro::Common::U32 size, const void *data, const bool reliable)
        {
            Kiaro::Common::U32 packet_flag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packet_flag = ENET_PACKET_FLAG_RELIABLE;

            ENetPacket *packet = enet_packet_create(data, size, packet_flag);
            enet_peer_send(this->Peer, 0, packet);
        }

        void OutgoingClient::Connect(const Kiaro::Common::C8 *target_address, const Kiaro::Common::U16 target_port, const Kiaro::Common::U32 wait)
        {
			// TODO (Robert MacGregor#9): Report Error
			if (this->Peer || this->Host)
				return;

            Kiaro::Logging::Write("OutgoingClient: Attempting connection to remote host %s:%u", Kiaro::Logging::LEVEL_INFO, 2, target_address, target_port);

            ENetAddress enet_address;
            enet_address_set_host(&enet_address, target_address);
            enet_address.port = target_port;

            this->Host = enet_host_create(NULL /* create a client host */,
                            1 /* only allow 1 outgoing connection */,
                            2 /* allow up 2 channels to be used, 0 and 1 */,
                            57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                            14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

            this->Peer = enet_host_connect(this->Host, &enet_address, 2, 0);

            ENetEvent event;
            if (enet_host_service(this->Host, &event, wait) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
            {
                this->OnConnected();
                this->Connected = true;
                //this->address = enet_address.host;
                this->Port = enet_address.port;
                return;
            }

            Kiaro::Logging::Write("OutgoingClient: Failed to connect to remote host", Kiaro::Logging::LEVEL_ERROR);

            this->OnConnectFailed();
            enet_peer_reset(this->Peer);
        }

        void OutgoingClient::Disconnect(const Kiaro::Common::U32 wait)
        {
            this->Connected = false;

            if (this->Host == 0x00 || !this->Connected)
                return;

            ENetEvent event;
			// TODO(Robert MacGregor#5): Wait for disconnect event that doesn't come immediately?
            while (enet_host_service(this->Host, &event, wait) > 0)
                switch (event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                    	enet_host_destroy(this->Host);
						this->Host = 0x00;
                        return;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                        enet_packet_destroy(event.packet);
                        break;
                    // Shutup compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                        break;
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }
        }

        void OutgoingClient::Update(void)
        {
            ENetEvent event;
            while (enet_host_service(this->Host, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        #ifdef _DEBUG
                        printf("KIARO NETWORK: Network Client has been Dropped ...\n");
                        #endif

                        this->OnDisconnected();
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        this->OnReceivePacket(event.packet);
                        break;
                    }

                    // Pipe down compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                        break;
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }
            }
        }

        inline void OutgoingClient::OnReceivePacket(ENetPacket *received_packet)
        {

        }

        inline void OutgoingClient::OnConnected(void)
        {

        }

        inline void OutgoingClient::OnDisconnected(void)
        {

        }

        inline void OutgoingClient::OnConnectFailed(void)
        {

        }

        OutgoingClient *OutgoingClient::Get(void)
        {
            if (!OutgoingClient_Singleton)
                OutgoingClient_Singleton = new OutgoingClient;

            return OutgoingClient_Singleton;
        }

        void OutgoingClient::Destroy(void)
        {
            if (OutgoingClient_Singleton)
            {
                delete OutgoingClient_Singleton;
                OutgoingClient_Singleton = 0x00;
            }
        }

        bool OutgoingClient::IsConnected(void)
        {
            return this->Connected;
        }

        void OutgoingClient::Dispatch(void) { if (this->Host) enet_host_flush(this->Host); }
    } // End Namespace Network
} // End Namespace Kiaro
