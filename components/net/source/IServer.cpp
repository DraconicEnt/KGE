/**
 *  @file ServerSingleton.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <support/Console.hpp>

#include <net/IServer.hpp>
#include <net/IIncomingClient.hpp>

namespace Kiaro
{
    namespace Net
    {
        IServer::IServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) :
        mLastPacketSender(NULL), mRunning(true), mInternalHost(NULL), mListenPort(listenPort), mListenAddress(listenAddress), mMaximumClientCount(maximumClientCount)
        {
            CONSOLE_INFOF("Creating server on %s:%u with %u maximum clients ...", listenAddress.data(), listenPort, maximumClientCount);

            ENetAddress enetAddress;
            enetAddress.port = listenPort;
            enet_address_set_host(&enetAddress, listenAddress.c_str());

            // FIXME (Robert MacGregor#9): Reliable dispatch of server full messages?
            mInternalHost = enet_host_create(&enetAddress, maximumClientCount + 1, 2, 0, 0);
            if (!mInternalHost)
            {
                mRunning = false;

                throw std::runtime_error("IServer: Failed to create ENet host!");
            }
        }

        IServer::~IServer(void)
        {
            CONSOLE_INFO("Deinitializing game server ...");

            // Disconnect everyone
            for (IIncomingClient* currentClient: mConnectedClientSet)
                currentClient->disconnect("Server Shutdown");

           // Game::SGameWorld::destroy();

            if (mInternalHost)
            {
                enet_host_flush(mInternalHost); // Make sure we dispatch disconnects
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            mRunning = false;
        }

        void IServer::globalSend(IMessage* packet, const bool& reliable)
        {
            for (IIncomingClient* currentClient: mConnectedClientSet)
                currentClient->send(packet, reliable);
        }

        void IServer::update(const Common::F32& deltaTimeSeconds)
        {
            // TODO (Robert MacGregor#9): Dispatch commit packets after we're done dispatching sim updates
           // Net::Messages::SimCommit commitPacket;
           // this->globalSend(&commitPacket, true);

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        CONSOLE_INFO("Received client connect challenge.");

                        IIncomingClient* client = this->onReceiveClientChallenge(event.peer);
                        event.peer->data = client;

                        mPendingClientSet.insert(mPendingClientSet.end(), client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        CONSOLE_INFO("Received client disconnect.");

                        IIncomingClient* disconnected = reinterpret_cast<IIncomingClient*>(event.peer->data);
                        onClientDisconnected(disconnected);
                        disconnected->mIsConnected = false;

                        // TODO (Robert MacGregor#9): Delete from the correct set
                        mConnectedClientSet.erase(disconnected);

                        delete disconnected;

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        if (!event.peer->data)
                        {
                            enet_packet_destroy(event.packet);
                            throw std::runtime_error("IServer: Invalid ENet peer data on packet receive!");
                        }

                        IIncomingClient* sender = reinterpret_cast<IIncomingClient*>(event.peer->data);

                        mLastPacketSender = sender;

                        Support::CBitStream incomingStream(event.packet->data, event.packet->dataLength);
                        this->processPacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        mLastPacketSender = nullptr;

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
        }

        void IServer::processPacket(Support::CBitStream& incomingStream, Net::IIncomingClient* sender)
        {
            assert(!incomingStream.isEmpty());

            this->onReceivePacket(incomingStream, sender);
        }

        void IServer::onClientConnected(IIncomingClient* client)
        {
          //  Core::SEventManager::get()->mOnClientConnectedEvent.invoke(client);
            //Core::Logging::write(Core::Logging::MESSAGE_INFO, "IServer: Received remote connection from %s:%u.", client->getStringIPAddress().data(), client->getPort());

            // FIXME (Robert MacGregor#9): Net some objects across their entire net sequence; perform only on proper client auth
           // Net::Messages::Scope scope;
          //  const Game::Entities::IEntity* const* activeEntities = Game::SGameWorld::getPointer()->getEntities();

           // for (Common::U32 iteration = 0; iteration < 4096; iteration++)
            //    if (activeEntities[iteration] && activeEntities[iteration]->getHintMask() & Game::Entities::NO_SCOPING)
            //        scope.add(activeEntities[iteration]);

            //client->send(&scope, true);
        }

        void IServer::onClientDisconnected(IIncomingClient* client)
        {
         //   Core::SEventManager::get()->mOnClientDisconnectedEvent.invoke(client);
            //Core::Logging::write(Core::Logging::MESSAGE_INFO, "IServer: Received disconnection from %s:%u.", client->getStringIPAddress().data(), client->getPort());
        }

        IIncomingClient* IServer::getLastPacketSender(void) noexcept
        {
            IIncomingClient* result = mLastPacketSender;
            mLastPacketSender = nullptr;

            return result;
        }

        void IServer::stop(void) noexcept { mRunning = false; }

        Common::U32 IServer::getClientCount(void) const noexcept
        {
            return mConnectedClientSet.size();
        }

        const bool& IServer::isRunning(void) const noexcept { return mRunning; }

        void IServer::dispatch(void)
        {
            if (mRunning)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Game
} // End Namespace Kiaro
