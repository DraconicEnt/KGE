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
            
            Entities::CTerrain* terrain = new Entities::CTerrain("textures/terrain-heightmap.bmp");
            terrain->registerEntity();
            
            // Add our update to the scheduler
            mUpdatePulse = Support::SSynchronousScheduler::getPointer()->schedule(32, true, this, &SGameServer::update);
        }

        SGameServer::~SGameServer(void)
        {
            // FIXME (Robert MacGregor#9): Call IServer destructor?
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
            Net::IServer::update(0);

            //mCurrentGamemode->
        }
        
        void SGameServer::onReceivePacket(Support::CBitStream& incomingStream, Net::IIncomingClient* sender)
        {
            // The packet whose payload is in incomingStream can contain multiple messages.
            // TODO: Alleviate DoS issues with a hard max on message counts
            while (!incomingStream.isEmpty())
            {
                Net::IMessage basePacket;
                basePacket.unpack(incomingStream);
                
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

                            default:
                            {
                                Support::Console::writef(Support::Console::MESSAGE_ERROR, "IServer: Unknown client stage: %u", sender->getConnectionStage());
                                break;
                            }
                        }
                    }
                }
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

                    Support::Console::writef(Support::Console::MESSAGE_INFO, "IServer: Client version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                    receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

                    Game::Messages::HandShake handShake;
                    sender->send(&handShake, true);

                    // At this point, the client has passed initial authentication
                    // TODO (Robert MacGregor#9): Make a proper challenge that isn't just version information.
                    Support::Console::write(Support::Console::MESSAGE_INFO, "IServer: Client passed initial authentication.");

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
                    Support::String exceptionText = "IServer: Out of stage or unknown message type encountered at stage 0 processing: ";
                    exceptionText += header.getType();
                    exceptionText += " for client <ADD IDENTIFIER> ";

                    throw std::out_of_range(exceptionText);
                    break;
                }
            }
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
