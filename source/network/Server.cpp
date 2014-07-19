/**
 *  @file Server.cpp
 *  @brief Source code for Server instances.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#include <exception>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <enet/enet.h>

#include <network/Server.h>
#include <network/IncomingClient.h>

namespace Kiaro
{
    namespace Network
    {
        Server::Server(const Kiaro::Common::C8 *listen_address, const Kiaro::Common::U16 listen_port, const Kiaro::Common::U32 clients) : is_running(true), Host(0x00)
        {
            ENetAddress enet_address;
            enet_address.port = listen_port;
            enet_address_set_host(&enet_address, listen_address);

            this->Address = enet_address.host;
            this->Port = enet_address.port;

            this->Host = enet_host_create(&enet_address, clients, 2, 0, 0);
			if (!this->Host)
			{
				this->is_running = false;
				return;
			}
        }

        Server::~Server(void)
        {
            if (this->Host)
            {
                enet_host_destroy(this->Host);
                this->Host = 0x00;
            }

            this->is_running = false;
        }

        void Server::Stop(void) { this->is_running = false; }

        bool Server::IsRunning(void) { return this->is_running; }

        void Server::Update(void)
        {
			if (!this->is_running)
        		return;

            ENetEvent event;
            while (enet_host_service(this->Host, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        IncomingClient *client = new Kiaro::Network::IncomingClient(event.peer, this);
                        event.peer->data = client;

                        this->OnClientConnected(client);
                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        this->OnClientDisconnected((IncomingClient*)event.peer->data);
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        // NOTE (Robert MacGregor#1): The Packet should be deleted in the IncomingClient.cpp code.
                        ((Kiaro::Network::IncomingClient*)event.peer->data)->OnReceivePacket(new Packet(event.packet));

                       // this->OnReceivePacket(incoming_packet);
                        break;
                    }
                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
        }

        void Server::Dispatch(void) { if (this->is_running) enet_host_flush(this->Host); }

        void Server::OnClientConnected(Kiaro::Network::IncomingClient *client)
        {

        }

        void Server::OnClientDisconnected(Kiaro::Network::IncomingClient *client)
        {

        }

        void Server::OnReceivePacket(Packet *packet)
        {

        }
    } // End Namespace Network
} // End Namespace Kiaro
