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

#include <engine/EntityGroupingSingleton.hpp>

#include <support/EventManagerSingleton.hpp>
#include <game/messages/messages.hpp>
#include <game/ServerSingleton.hpp>

#include <game/entities/Sky.hpp>

namespace Kiaro
{
    namespace Game
    {
        ServerSingleton *ServerSingleton_Instance = NULL;

        ServerSingleton *ServerSingleton::getPointer(const Kiaro::Common::String &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount)
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

        ServerSingleton::ServerSingleton(const Kiaro::Common::String &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount) : ServerBase(listenAddress, listenPort, maximumClientCount),
        mLastPacketSender(NULL)
        {
            // Create the map division
            Kiaro::Support::MapDivision::Get(12);

            mEntityGroup = Kiaro::Engine::EntityGroupingSingleton::getPointer();

            // All entities register themselves with the Entity grouping singleton
            new Kiaro::Game::Entities::Sky();

            // Create an init a gamemode
        }

        ServerSingleton::~ServerSingleton(void)
        {
            Kiaro::Engine::EntityGroupingSingleton::destroy();
        }

        void ServerSingleton::update(const Kiaro::Common::F32 &deltaTimeSeconds)
        {
            Kiaro::Network::ServerBase::update(deltaTimeSeconds);

            mEntityGroup->update(deltaTimeSeconds);
        }

        void ServerSingleton::onClientConnected(Kiaro::Network::IncomingClientBase *client)
        {
            Kiaro::Support::EventManagerSingleton::get()->mOnClientConnectedEvent.invoke(client);
            std::cout << "ServerSingleton: Received remote connection from x.x.x.x:" << client->getPort() << std::endl;
        }

        void ServerSingleton::onClientDisconnected(Kiaro::Network::IncomingClientBase *client)
        {
            Kiaro::Support::EventManagerSingleton::get()->mOnClientDisconnectedEvent.invoke(client);
            std::cout << "ServerSingleton: Received disconnection from x.x.x.x:" << client->getPort() << std::endl;
        }

        void ServerSingleton::onReceivePacket(Kiaro::Support::BitStream &incomingStream, Kiaro::Network::IncomingClientBase *sender)
        {
            mLastPacketSender = sender;

            Kiaro::Network::MessageBase basePacket;
            basePacket.unpackData(incomingStream);

            switch (basePacket.getType())
            {
                case Kiaro::Game::Packets::PACKET_HANDSHAKE:
                {
                    Kiaro::Game::Packets::HandShake receivedHandshake;
                    receivedHandshake.unpackData(incomingStream);

                    std::cout << "ServerSingleton: Client Version is " << (Kiaro::Common::U32)receivedHandshake.mVersionMajor << "."
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
    } // End Namespace Game
} // End Namespace Kiaro
