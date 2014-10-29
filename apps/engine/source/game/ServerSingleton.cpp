/**
 *  @file ServerSingleton.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <game/packets/packets.hpp>
#include <game/ServerSingleton.hpp>

namespace Kiaro
{
    namespace Game
    {
        ServerSingleton *ServerSingleton_Instance = NULL;

        ServerSingleton *ServerSingleton::getPointer(const std::string &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount)
        {
            if (!ServerSingleton_Instance)
                ServerSingleton_Instance = new ServerSingleton(listenAddress, listenPort, maximumClientCount);

            return ServerSingleton_Instance;
        }

        void ServerSingleton::destroy(void)
        {
            if (ServerSingleton_Instance)
                delete ServerSingleton_Instance;

            ServerSingleton_Instance = NULL;
        }

        ServerSingleton::ServerSingleton(const std::string &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount) : ServerBase(listenAddress, listenPort, maximumClientCount),
        mLastPacketSender(NULL)
        {
            // Create the map division
            Kiaro::Support::MapDivision::Get(12);
        }

        ServerSingleton::~ServerSingleton(void)
        {

        }

        void ServerSingleton::onClientConnected(Kiaro::Network::IncomingClientBase *client)
        {
            std::cout << "Server: Received remote connection from x.x.x.x:" << client->getPort() << std::endl;
        }

        void ServerSingleton::onClientDisconnected(Kiaro::Network::IncomingClientBase *client)
        {
            std::cout << "Server: Received disconnection from x.x.x.x:" << client->getPort() << std::endl;
        }

        void ServerSingleton::onReceivePacket(Kiaro::Support::BitStream &incomingStream, Kiaro::Network::IncomingClientBase *sender)
        {
            mLastPacketSender = sender;

            Kiaro::Network::PacketBase basePacket;
            basePacket.unpackData(incomingStream);

            switch (basePacket.getType())
            {
                case Kiaro::Game::Packets::PACKET_HANDSHAKE:
                {
                    Kiaro::Game::Packets::HandShake receivedHandshake;
                    receivedHandshake.unpackData(incomingStream);

                    std::cout << "Server: Client Version is " << (Kiaro::Common::U32)receivedHandshake.mVersionMajor << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionMinor << "." << (Kiaro::Common::U32)receivedHandshake.mVersionRevision << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionBuild << std::endl;

                    Kiaro::Game::Packets::HandShake handShake;
                    handShake.mVersionMajor = 1;
                    handShake.mVersionMinor = 2;
                    handShake.mVersionRevision = 3;
                    handShake.mVersionBuild = 4;

                    sender->send(&handShake, true);
                }
            }

            mLastPacketSender = NULL;
        }

        Kiaro::Network::IncomingClientBase *ServerSingleton::getLastPacketSender(void)
        {
            Kiaro::Network::IncomingClientBase *result = mLastPacketSender;
            mLastPacketSender = NULL;

            return result;
        }

        void ServerSingleton::addStaticEntity(Kiaro::Game::Entities::EntityBase *entity)
        {
            mStaticEntitySet.insert(entity);
        }
    } // End Namespace Game
} // End Namespace Kiaro
