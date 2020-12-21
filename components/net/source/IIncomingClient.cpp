/**
 *  @file IIncomingClient.cpp
 *  @brief Source code file defining logic for the Net::IIncomingClient class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <net/IIncomingClient.hpp>

#include <net/IMessage.hpp>
#include <net/config.hpp>

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Net
    {
        IIncomingClient::IIncomingClient(ENetPeer* connecting, IServer* server) : mInternalClient(connecting), mCurrentConnectionStage(STAGE_AUTHENTICATION),
        mIsConnected(true), mReliableStream(NETSTREAM_DEFAULT_SIZE, nullptr, 0, NETSTREAM_RESIZE_FACTOR), mUnreliableStream(NETSTREAM_DEFAULT_SIZE, nullptr, 0, NETSTREAM_RESIZE_FACTOR)
        {
        }

        IIncomingClient::~IIncomingClient(void)
        {
            if (mIsConnected)
                this->disconnect("Destroyed net handle.");
        }

        void IIncomingClient::send(const IMessage* packet, const bool reliable)
        {
            if (reliable)
            {
                packet->packEverything(mReliableStream);
            }
            else
            {
                packet->packEverything(mUnreliableStream);
            }
        }

        void IIncomingClient::send(const IMessage& message, const bool reliable)
        {
            this->send(&message, reliable);
        }

        void IIncomingClient::disconnect(const Support::String& reason)
        {
            enet_peer_disconnect_later(mInternalClient, 0);
        }

        Common::U16 IIncomingClient::getPort(void) const
        {
            return mInternalClient->address.port;
        }

        Common::U32 IIncomingClient::getIPAddress(void) const
        {
            return mInternalClient->address.host;
        }

        Support::String IIncomingClient::getIPAddressString(void) const
        {
            Common::C8 temporaryBuffer[18];
            enet_address_get_host_ip(&mInternalClient->address, temporaryBuffer, 18);

            return Support::String(temporaryBuffer);
        }

        void IIncomingClient::setConnectionStage(const STAGE_NAME& in)
        {
            mCurrentConnectionStage = in;
        }

        const STAGE_NAME& IIncomingClient::getConnectionStage(void) const
        {
            return mCurrentConnectionStage;
        }

        void IIncomingClient::dispatchQueuedMessages(void)
        {
            ENetPacket* enetPacket = nullptr;

            if (mReliableStream.getPointer() != 0)
            {
                enetPacket = enet_packet_create(mReliableStream.getBlock(), mReliableStream.getPointer(), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(mInternalClient, 0, enetPacket);
            }

            if (mUnreliableStream.getPointer() != 0)
            {
                enetPacket = enet_packet_create(mUnreliableStream.getBlock(), mUnreliableStream.getPointer(), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
                enet_peer_send(mInternalClient, 0, enetPacket);
            }

            mReliableStream.setPointer(0);
            mUnreliableStream.setPointer(0);
        }
    } // End Namespace Network
} // End Namespace Kiaro
