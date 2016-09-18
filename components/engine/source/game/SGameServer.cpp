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
            handShake.mDataBlockCount = mDataBlocks.size();
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

            // Dispatch everything we have queued
            for (Net::IIncomingClient* client: mConnectedClientSet)
                client->dispatchQueuedMessages();
        }

        void SGameServer::onClientDisconnected(Net::IIncomingClient* client)
        {
            auto searchResult = mQueuedStreams.find(client);

            if (searchResult != mQueuedStreams.end())
            {
                // Nuke anything in the queued streams
                while (mQueuedStreams[client].size() != 0)
                {
                    delete mQueuedStreams[client].front();
                    mQueuedStreams[client].pop();
                }

                mQueuedStreams.erase(client);
            }
        }

        void SGameServer::onReceivePacket(Support::CBitStream& incomingStream, Net::IIncomingClient* sender)
        {
            Core::SEngineInstance* engine = Core::SEngineInstance::getPointer();
            Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

            const Common::U32 messageLimit = settings->getValue<Common::U32>("Server::MessagesPerTick");
            const Common::U32 queueLimit = settings->getValue<Common::U32>("Server::MaxQueuedStreams");

            Support::CBitStream& processedStream = incomingStream;

            // If we still have queued streams, make sure we're not exceeding the max stream count
            if (queueLimit != 0)
            {
                auto searchResult = mQueuedStreams.find(sender);

                if (searchResult != mQueuedStreams.end())
                {
                    // Too much queued data?
                    if (mQueuedStreams[sender].size() > queueLimit)
                    {
                        // We deal with any queued streams they might have in the disconnect routine
                        sender->disconnect("Too much queued data.");
                        return;
                    }

                    // If the queued stream list isn't empty, we process the first one first
                    if (mQueuedStreams[sender].size() != 0)
                        processedStream = *mQueuedStreams[sender].front();
                }
            }

            // Process messages in our stream up to the message count
            for (Common::U32 iteration = 0; (messageLimit != 0 && iteration < messageLimit) && !incomingStream.isFull(); ++iteration)
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
                Support::throwFormattedException<std::out_of_range>("SGameServer: Out of stage or unknown message type encountered at stage 0 processing: %u for client <IDENT>", basePacket.getType());
            }

            // If the stream still isn't done, queue it up (and it's not already in the queue...)
            if (!incomingStream.isFull() && &processedStream == &incomingStream)
            {
                auto searchResult = mQueuedStreams.find(sender);

                if (searchResult == mQueuedStreams.end())
                    mQueuedStreams[sender] = Support::Queue<Support::CBitStream*>();

                Support::CBitStream* streamCopy = new Support::CBitStream(incomingStream);
                mQueuedStreams[sender].push(streamCopy);
            }
            else if (incomingStream.isFull() && &processedStream != &incomingStream) // But the stream is done and it's a queued stream, we delete it
            {
                mQueuedStreams[sender].pop();
                delete &processedStream;
            }
        }

        bool SGameServer::addDataBlock(Game::Entities::DataBlocks::IDataBlock* datablock)
        {
            // Must be not null
            assert(datablock);
            // Must not already be registered
            assert(mDataBlocks.find(datablock) == mDataBlocks.end());

            if (!datablock->validate())
                return false;

            mDataBlocks.insert(mDataBlocks.end(), datablock);
            return true;
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
