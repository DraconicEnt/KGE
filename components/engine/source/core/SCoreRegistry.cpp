/**
 *  @file SCoreRegistry.cpp
 *  @brief Source file implementing the SCoreRegistry class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/messages/messages.hpp>
#include <core/SCoreRegistry.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            Game::IEntity* SCoreRegistry::constructEntity(const Common::U32 id, Support::CBitStream& payload)
            {
                auto search = mEntityTypeMap.find(id);

                if (search == mEntityTypeMap.end())
                {
                    return nullptr;
                }

                return (*search).second(payload);
            }

            SCoreRegistry::SCoreRegistry(void) : mMessageTypeCounter(0), mEntityTypeCounter(0)
            {
                this->registerMessages();
                this->registerEntityTypes();

                CONSOLE_INFOF("Initialized with %u network message types, %u entity types.", mMessageMap.size(), mEntityTypeMap.size());
            }

            SCoreRegistry::~SCoreRegistry(void)
            {

            }

            SCoreRegistry::MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer> SCoreRegistry::lookupServerMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id)
            {
                auto searchResult = mServerStageMap[stage].find(id);

                if (searchResult == mServerStageMap[stage].end())
                {
                    return nullptr;
                }

                return (*searchResult).second.second;
            }

            SCoreRegistry::MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient> SCoreRegistry::lookupClientMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id)
            {
                auto searchResult = mClientStageMap[stage].find(id);

                if (searchResult == mClientStageMap[stage].end())
                {
                    return nullptr;
                }

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

            void SCoreRegistry::registerEntityTypes(void)
            {

            }
        } // End NameSpace Core
    } // End NameSpace Engine
} // End NameSpace Kiaro
