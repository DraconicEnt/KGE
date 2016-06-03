/**
 */

#include <support/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <game/IGameMode.hpp>

#include <game/entities/entities.hpp>

#include <game/CGameClient.hpp>
#include <game/SGameWorld.hpp>

#include <game/messages/messages.hpp>

#include <support/Console.hpp>

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

        SGameServer::SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) : Net::IServer(listenAddress, listenPort, maximumClientCount),
        mMessageCounter(0)
        {
            mSimulation = new Phys::CSimulation();

            // Add our update to the scheduler
            mUpdatePulse = Support::SSynchronousScheduler::getPointer()->schedule(32, true, this, &SGameServer::update);

            // Initialize the messages
            Common::U32 messageCounter = 0;

            this->registerMessage(Net::IMessage::constructMessage<Game::Messages::Disconnect>, &SGameServer::handshakeHandler, Net::STAGE_UNSTAGED);

            this->registerMessage(Net::IMessage::constructMessage<Game::Messages::HandShake>, &SGameServer::handshakeHandler, Net::STAGE_AUTHENTICATION);
        }

        void SGameServer::registerMessage(TestSet::StaticDelegateFuncPtr messageConstructor, MessageHandlerSet::MemberDelegateFuncPtr<SGameServer> handler, const Net::STAGE_NAME stage)
        {
            mMessageMap[mMessageCounter] = messageConstructor;
            mStageMap[stage][mMessageCounter] = std::make_pair(messageConstructor, handler);

            ++mMessageCounter;
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
            // FIXME (Robert MacGregor#9): Call IServer destructor?
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
            // The packet whose payload is in incomingStream can contain multiple messages.
            // TODO: Alleviate DoS issues with a hard max on message counts
            while (!incomingStream.isFull())
            {
                Net::IMessage basePacket;
                basePacket.unpack(incomingStream);

                // Once we know at least the ID, first look it up as a stageless message
                auto searchResult = mStageMap[Net::STAGE_UNSTAGED].find(basePacket.getType());

                if (searchResult != mStageMap[Net::STAGE_UNSTAGED].end())
                {
                    // Execute the appropriate responder
                    MessageHandlerSet::MemberDelegateFuncPtr<SGameServer> responder = (*searchResult).second.second;

                    (this->*responder)(sender, incomingStream);

                    continue;
                }

                // If we got to this point, look it up by the client's stage
                const Common::U8 senderStage = sender->getConnectionStage();
                searchResult = mStageMap[senderStage].find(basePacket.getType());

                if (searchResult != mStageMap[senderStage].end())
                {
                    // Execute the appropriate responder
                    MessageHandlerSet::MemberDelegateFuncPtr<SGameServer> responder = (*searchResult).second.second;

                    (this->*responder)(sender, incomingStream);

                    continue;
                }

                // Not a valid message
                // TODO (Robert MacGregor#9): IP Address
                Support::String exceptionText = "SGameServer: Out of stage or unknown message type encountered at stage 0 processing: ";

                exceptionText += basePacket.getType();
                exceptionText += " for client <ADD IDENTIFIER> ";

                throw std::out_of_range(exceptionText);

                /*
                switch (basePacket.getType())
                {
                    // Stageless messages

                    // If it's not any stageless message, then drop into the appropriate stage handler
                    default:
                    {
                        switch (sender->getConnectionStage())
                        {
                            case Net::STAGE_AUTHENTICATION:
                            {
                                this->processStageZero(basePacket, incomingStream, sender);
                                break;
                            }

                            case Net::STAGE_LOADING:
                            {
                                Support::Console::error("Unimplemented");
                                break;
                            }

                            default:
                            {
                                CONSOLE_ERRORF("Unknown client stage: %u", sender->getConnectionStage());
                                break;
                            }
                        }
                    }
                }
                */
            }
        }

        void SGameServer::processStageZero(const Net::IMessage& header, Support::CBitStream& incomingStream, Net::IIncomingClient* sender)
        {
            switch(header.getType())
            {
                case Game::Messages::TYPE_HANDSHAKE:
                {
                    Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.unpack(incomingStream);
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
                    break;
                }

                // Out of stage message or a totally unknown message type
                default:
                {
                    // TODO (Robert MacGregor#9): IP Address
                    Support::String exceptionText = "SGameServer: Out of stage or unknown message type encountered at stage 0 processing: ";
                    exceptionText += header.getType();
                    exceptionText += " for client <ADD IDENTIFIER> ";

                    throw std::out_of_range(exceptionText);
                    break;
                }
            }
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
