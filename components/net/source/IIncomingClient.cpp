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
        IIncomingClient::IIncomingClient(ENetPeer* connecting, IServer* server) : mInternalClient(connecting), mIsOppositeEndian(false),
        mCurrentConnectionStage(STAGE_AUTHENTICATION), mIsConnected(true), mOutputBitStream(NETSTREAM_DEFAULT_SIZE, nullptr, 0, NETSTREAM_RESIZE_FACTOR)
        {
        }

        IIncomingClient::~IIncomingClient(void)
        {
            if (mIsConnected)
                this->disconnect("Destroyed net handle.");
        }

        void IIncomingClient::send(IMessage* packet, const bool reliable)
        {
            Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;

            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // Write the packet data to our stream
            packet->packEverything(mOutputBitStream);
            ENetPacket* enetPacket = enet_packet_create(mOutputBitStream.getBlock(), mOutputBitStream.getPointer(), packetFlag);

            // TODO: Optionally dispatch this immediately, otherwise we queue up in the same ENet packet
            enet_peer_send(mInternalClient, 0, enetPacket);

            mOutputBitStream.setPointer(0);
        }

        bool IIncomingClient::getIsOppositeEndian(void) const
        {
            return mIsOppositeEndian;
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
    } // End Namespace Network
} // End Namespace Kiaro
