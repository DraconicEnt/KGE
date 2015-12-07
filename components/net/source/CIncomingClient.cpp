/**
 *  @file CIncomingClient.cpp
 *  @brief Source code file defining logic for the Net::CIncomingClient class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <net/CIncomingClient.hpp>

#include <net/messages/messages.hpp>

#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Net
    {
        CIncomingClient::CIncomingClient(ENetPeer* connecting, IServer* server) : mInternalClient(connecting), mIsOppositeEndian(false),
        mCurrentConnectionStage(STAGE_AUTHENTICATION), mIsConnected(true)
        {

        }

        CIncomingClient::~CIncomingClient(void)
        {
            if (mIsConnected)
                this->disconnect("Destroyed net handle.");
        }

        void CIncomingClient::send(Messages::IMessage* packet, const bool& reliable)
        {
            Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;

            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Support::CBitStream outStream(packet);

            ENetPacket* enetPacket = enet_packet_create(outStream.getBlock(), outStream.getPointer(), packetFlag);
            enet_peer_send(mInternalClient, 0, enetPacket);
        }

        bool CIncomingClient::getIsOppositeEndian(void)
        {
            return mIsOppositeEndian;
        }

        void CIncomingClient::disconnect(const Support::String& reason)
        {
            Net::Messages::Disconnect disconnect;
            disconnect.mReason = reason;

            this->send(&disconnect, true);
            enet_peer_disconnect_later(mInternalClient, 0);
        }

        const Common::U16& CIncomingClient::getPort(void)
        {
            return mInternalClient->address.port;
        }

        const Common::U32& CIncomingClient::getIPAddress(void)
        {
            return mInternalClient->address.host;
        }

        Support::String CIncomingClient::getIPAddressString(void)
        {
            // TODO (Robert MacGregor#9): Make sure that the STL handles this correctly?
            Common::C8 temporaryBuffer[18];
            enet_address_get_host_ip(&mInternalClient->address, temporaryBuffer, 18);

            return temporaryBuffer;
        }

        void CIncomingClient::setConnectionStage(const STAGE_NAME& in)
        {
            mCurrentConnectionStage = in;
        }

        const STAGE_NAME& CIncomingClient::getConnectionStage(void)
        {
            return mCurrentConnectionStage;
        }
    } // End Namespace Network
} // End Namespace Kiaro
