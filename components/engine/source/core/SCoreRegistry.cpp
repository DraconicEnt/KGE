/**
*/

#include <game/messages/messages.hpp>
#include <core/SCoreRegistry.hpp>

#include <support/Console.hpp>

#include <game/entities/datablocks/datablocks.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            static SCoreRegistry* sInstance = nullptr;

            SCoreRegistry* SCoreRegistry::getPointer(void)
            {
                if (!sInstance)
                    sInstance = new SCoreRegistry();

                return sInstance;
            }

            void SCoreRegistry::destroy(void)
            {
                delete sInstance;
                sInstance = nullptr;
            }

            Game::Entities::IEntity* SCoreRegistry::constructEntity(const Common::U32 id, Support::CBitStream& payload)
            {
                auto search = mEntityTypeMap.find(id);

                if (search == mEntityTypeMap.end())
                    return nullptr;

                return (*search).second(payload);
            }

            SCoreRegistry::SCoreRegistry(void) : mMessageTypeCounter(0), mDataBlockTypeCounter(0), mEntityTypeCounter(0)
            {
                this->registerMessages();
                this->registerEntityTypes();
                this->registerDatablockTypes();

                CONSOLE_INFOF("Initialized with %u network message types, %u datablock types, %u entity types.", mMessageMap.size(), mDatablockTypeMap.size(), mEntityTypeMap.size());
            }

            SCoreRegistry::~SCoreRegistry(void)
            {

            }

            SCoreRegistry::MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer> SCoreRegistry::lookupServerMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id)
            {
                auto searchResult = mServerStageMap[stage].find(id);

                if (searchResult == mServerStageMap[stage].end())
                    return nullptr;

                return (*searchResult).second.second;
            }

            SCoreRegistry::MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient> SCoreRegistry::lookupClientMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id)
            {
                auto searchResult = mClientStageMap[stage].find(id);

                if (searchResult == mClientStageMap[stage].end())
                    return nullptr;

                return (*searchResult).second.second;
            }

            void SCoreRegistry::registerMessages(void)
            {
                // Authentication Stage registration
                this->registerMessage<Game::Messages::HandShake>(&Game::SGameServer::handshakeHandler, &Core::COutgoingClient::handshakeHandler, Net::STAGE_AUTHENTICATION);

                // Loading stage registration
                this->registerMessage<Game::Messages::Scope>(nullptr, &Core::COutgoingClient::scopeHandler, Net::STAGE_LOADING);
                this->registerMessage<Game::Messages::SimCommit>(nullptr, &Core::COutgoingClient::simCommitHandler, Net::STAGE_LOADING);
            }

            void SCoreRegistry::registerDatablockTypes(void)
            {
                this->registerDataBlockType<Game::Entities::DataBlocks::CPlayerData>();
            }

            void SCoreRegistry::registerEntityTypes(void)
            {
                this->registerEntityType<Game::Entities::CTerrain>();
            }
        } // End NameSpace Core
    } // End NameSpace Engine
} // End NameSpace Kiaro
