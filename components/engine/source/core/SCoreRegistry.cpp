/**
*/

#include <game/messages/messages.hpp>
#include <core/SCoreRegistry.hpp>

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

            SCoreRegistry::SCoreRegistry(void)
            {
                // Authentication Stage registration
                this->registerMessage<Game::Messages::HandShake>(&Game::SGameServer::handshakeHandler, &Core::COutgoingClient::handshakeHandler, Net::STAGE_AUTHENTICATION);

                // Loading stage registration
                this->registerMessage<Game::Messages::Scope>(nullptr, &Core::COutgoingClient::scopeHandler, Net::STAGE_LOADING);
                this->registerMessage<Game::Messages::SimCommit>(nullptr, &Core::COutgoingClient::simCommitHandler, Net::STAGE_LOADING);

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
        } // End NameSpace Core
    } // End NameSpace Engine
} // End NameSpace Kiaro
