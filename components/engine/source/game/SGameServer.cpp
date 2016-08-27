/**
 *  @file SGameServer.cpp
 *  @brief Source code file implementing the SGameServer singleton class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <game/IGameMode.hpp>

#include <game/entities/entities.hpp>

#include <game/CGameClient.hpp>
#include <game/SGameWorld.hpp>

#include <game/messages/messages.hpp>

#include <support/SProfiler.hpp>

namespace Kiaro
{
    namespace Game
    {
        static SGameServer* sInstance = nullptr;

        void SGameServer::initialize(void)
        {
            if (!sInstance)
            {
                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();
                sInstance = new SGameServer(settings->getValue<Support::String>("Server::ListenAddress"),
                                            settings->getValue<Common::U16>("Server::ListenPort"),
                                            settings->getValue<Common::U32>("Server::MaximumClientCount"));
            }
        }

        SGameServer* SGameServer::getPointer(void)
        {
            return sInstance;
        }

        void SGameServer::destroy(void)
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = nullptr;
            }
        }

        Net::IIncomingClient* SGameServer::onReceiveClientChallenge(Net::RemoteHostContext client)
        {
            CGameClient* incoming = new CGameClient(client);
            return incoming;
        }

        SGameServer::SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) : Net::IServer(listenAddress, listenPort, maximumClientCount)
        {
            mSimulation = new Phys::CSimulation();

            // Add our update to the scheduler
            mUpdatePulse = Support::SSynchronousScheduler::getPointer()->schedule(32, true, this, &SGameServer::update);
        }

        void SGameServer::handshakeHandler(Net::IIncomingClient* sender, Support::CBitStream& in)
        {
            Game::Messages::HandShake receivedHandshake;
            receivedHandshake.unpack(in);
            CONSOLE_INFOF("Client version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                          receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

            Game::Messages::HandShake handShake;
            sender->send(&handShake, true);

            // At this point, the client has passed initial authentication
            // TODO (Robert MacGregor#9): Make a proper challenge that isn't just version information.
            CONSOLE_INFO("Client passed initial authentication.");
            mPendingClientSet.erase(sender);
            mConnectedClientSet.insert(mConnectedClientSet.end(), sender);
            sender->setConnectionStage(Net::STAGE_LOADING);

            this->onClientConnected(sender);
        }

        SGameServer::~SGameServer(void)
        {
            assert(mSimulation);
            assert(mUpdatePulse);

            delete mSimulation;

            mSimulation = nullptr;
            mUpdatePulse->cancel();

            mUpdatePulse = nullptr;
        }

        void SGameServer::onClientConnected(Net::IIncomingClient* client)
        {
            Net::IServer::onClientConnected(client);
            this->initialScope(client);
        }

        void SGameServer::initialScope(Net::IIncomingClient* client)
        {
            Game::Messages::Scope scope;
            Game::SGameWorld* world = Game::SGameWorld::getPointer();

            for (auto it = world->begin(); it != world->end(); it++)
            {
                Entities::IEntity* entity = *it;

                if (entity->mFlags & Entities::FLAG_SCOPING || entity->mFlags & Entities::FLAG_STATIC)
                    scope.add(entity);
            }

            client->send(&scope, true);
        }

        void SGameServer::setGamemode(IGameMode* game)
        {
            if (mCurrentGamemode)
            {
                mCurrentGamemode->tearDown();
                delete mCurrentGamemode;
            }

            mCurrentGamemode = game;

            if (mCurrentGamemode)
                mCurrentGamemode->setup();
        }

        void SGameServer::update(void)
        {
            PROFILER_BEGIN(Server);
            Net::IServer::update(0);
            PROFILER_END(Server);
            //mCurrentGamemode->
        }

        void SGameServer::onReceivePacket(Support::CBitStream& incomingStream, Net::IIncomingClient* sender)
        {
            Core::SEngineInstance* engine = Core::SEngineInstance::getPointer();

            // The packet whose payload is in incomingStream can contain multiple messages.
            // TODO: Alleviate DoS issues with a hard max on message counts
            while (!incomingStream.isFull())
            {
                Net::IMessage basePacket;
                basePacket.unpack(incomingStream);

                Core::SEngineInstance::MessageHandlerSet::MemberDelegateFuncPtr<SGameServer> responder = engine->lookupServerMessageHandler(Net::STAGE_UNSTAGED, basePacket.getType());

                if (responder)
                {
                    (this->*responder)(sender, incomingStream);
                    continue;
                }

                // If we got to this point, look it up by the client's stage
                responder = engine->lookupServerMessageHandler(sender->getConnectionStage(), basePacket.getType());
                if (responder)
                {
                    (this->*responder)(sender, incomingStream);
                    continue;
                }

                // Not a valid message
                // TODO (Robert MacGregor#9): IP Address
                Support::String exceptionText = "SGameServer: Out of stage or unknown message type encountered at stage 0 processing: ";

                exceptionText += basePacket.getType();
                exceptionText += " for client <ADD IDENTIFIER> ";

                throw std::out_of_range(exceptionText);
            }
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
