/**
 *  @file SClient.cpp
 *  @brief Implementation file for the Network::SClient class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <enet/enet.h>

#include <support/Console.hpp>

#include <net/IOutgoingClient.hpp>

#include <support/CBitStream.hpp>

#include <net/IMessage.hpp>
#include <net/stages.hpp>

namespace Kiaro
{
    namespace Net
    {
        IOutgoingClient::IOutgoingClient() : mPort(0), mCurrentStage(0), mConnected(false), mInternalPeer(nullptr), mInternalHost(nullptr)
        {

        }

        IOutgoingClient::~IOutgoingClient(void)
        {
            if (mInternalPeer)
            {
                enet_peer_reset(mInternalPeer);
                mInternalPeer = nullptr;
            }

            if (mInternalHost)
            {
                enet_host_destroy(mInternalHost);
                mInternalHost = nullptr;
            }
        }

		const bool& IOutgoingClient::isOppositeEndian(void) const NOEXCEPT
        {
            return mOppositeEndian;
        }

        void IOutgoingClient::send(IMessage* packet, const bool& reliable)
        {
            Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Support::CBitStream outStream(packet);

            ENetPacket* enetPacket = enet_packet_create(outStream.getBlock(), outStream.getPointer(), packetFlag);
            enet_peer_send(mInternalPeer, 0, enetPacket);
        }

        void IOutgoingClient::processPacket(Support::CBitStream& incomingStream)
        {
            assert(!incomingStream.isEmpty());

            this->onReceivePacket(incomingStream);
        }

		const Common::U16& IOutgoingClient::getPort(void) const NOEXCEPT
        {
            return mPort;
        }

        void IOutgoingClient::connect(const Support::String& hostName, const Common::U16& targetPort, const Common::U32& wait)
        {
			// TODO: Report Error
			if (mInternalPeer || mInternalHost)
				return;

            // FIXME (Robert MacGregor#9): IP Octets >= 256
            // FIXME (Robert MacGregor#9): DNS Names (host.host.net)
            // TODO (Robert MacGregor#9): Update for std::regex
            //static const std::regex ipExpression("([0-9]+.[0-9]+.[0-9]+.[0-9])|([[:alpha:]]+.([[:alpha:]]+.)*[[:alpha:]]+)");
            //if (!std::regex_match(targetAddress, ipExpression))
            //{
            //    std::cerr << "OutgoingClientBase: Invalid remote host '" << targetAddress << "'!" << std::endl;
            //    return;
           // }

      //      Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Attempting connection to remote host %s:%u.", targetAddress.data(), targetPort);

            ENetAddress enetAddress;
            enet_address_set_host(&enetAddress, hostName.c_str());
            enetAddress.port = targetPort;

            mInternalHost = enet_host_create(nullptr /* create a client host */,
                            1 /* only allow 1 outgoing connection */,
                            2 /* allow up 2 channels to be used, 0 and 1 */,
                            57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                            14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

            mInternalPeer = enet_host_connect(mInternalHost, &enetAddress, 2, 0);

            ENetEvent event;
            if (enet_host_service(mInternalHost, &event, wait) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
            {
                mCurrentStage = STAGE_AUTHENTICATION;

                mConnected = true;
                mPort = targetPort;
                this->onConnected();

                // Add our update to the scheduler
                mUpdatePulse = Support::SSynchronousScheduler::getPointer()->schedule(32, true, this, &IOutgoingClient::update);
                return;
            }

            Support::Console::write(Support::Console::MESSAGE_ERROR, "SEngineInstance: Failed to connect to remote host.");

            this->onConnectFailed();
            enet_peer_reset(mInternalPeer);
        }

        void IOutgoingClient::disconnect(void)
        {
            if (mUpdatePulse)
                mUpdatePulse->cancel();

            mUpdatePulse = nullptr;

            if (!mConnected)
                return;

            mConnected = false;
            enet_peer_disconnect_later(mInternalPeer, 0);
        }

        void IOutgoingClient::update(void)
        {
            if (!mConnected && !mInternalPeer)
                return;

            ENetEvent event;
            while (mInternalHost && enet_host_service(mInternalHost, &event, 0) > 0)
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        enet_host_destroy(mInternalHost);
                        mInternalHost = nullptr;
                        enet_peer_reset(mInternalPeer);
                        mInternalPeer = nullptr;

                        this->onDisconnected();
                        CONSOLE_INFO("Disconnected from remote host.");

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        // We're disconnecting, so just destroy anything we receive in the meantime.
                        if (!mConnected)
                        {
                            enet_packet_destroy(event.packet);
                            break;
                        }

                        Support::CBitStream incomingStream(event.packet->data, event.packet->dataLength);
                        this->processPacket(incomingStream);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    // Pipe down compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }
        }

        const bool& IOutgoingClient::isConnected(void)
        {
            return mConnected;
        }

        void IOutgoingClient::dispatch(void)
        {
            if (mInternalHost)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Network
} // End Namespace Kiaro
