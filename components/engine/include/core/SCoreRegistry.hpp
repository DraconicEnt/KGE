/**
 *  @file IEntity.cpp
 *  @brief Source file implementing the IEntity interface class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_
#define _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_

#include <support/ISingleton.hpp>

#include <game/SGameServer.hpp>
#include <core/COutgoingClient.hpp>
#include <game/entities/datablocks/IDataBlock.hpp>
#include <game/entities/entities.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            /**
             *  @brief A singleton class representing the central type registrations for messages and datablocks.
             *  @detail This is used heavily by the netcode to do message & datablock lookups to find the appropriate constructor methods. These constructor
             *  methods would then be on their own devices to make sense of the current state of the incoming bitstream.
             */
            class SCoreRegistry : public Support::ISingleton<SCoreRegistry>
            {
                public:
                    //! A typedef representing a pointer to a message constructor.
                    typedef EasyDelegate::DelegateSet<Net::IMessage*, Support::CBitStream&>::StaticDelegateFuncPtr MessageConstructorPointer;
                    //! A typedef representing a pointer to a network datablock constructor.
                    typedef EasyDelegate::DelegateSet<Game::Entities::DataBlocks::IDataBlock*, Support::CBitStream&>::StaticDelegateFuncPtr NetworkDataBlockConstructorPointer;
                    //! A typedef representing a pointer to a entity constructor.
                    typedef EasyDelegate::DelegateSet<Game::Entities::IEntity*, Support::CBitStream&>::StaticDelegateFuncPtr NetworkEntityConstructorPointer;
                    //! A typedef representing a pointer to a delegate set of message handlers.
                    typedef EasyDelegate::DelegateSet<void, Net::IIncomingClient*, Support::CBitStream&> MessageHandlerSet;

                private:
                    //! The current counter value for message types.
                    Common::U32 mMessageTypeCounter;
                    //! The current counter value for datablock types.
                    Common::U32 mDataBlockTypeCounter;
                    //! The current counter value for network entity types.
                    Common::U32 mEntityTypeCounter;

                    //! The message mapping for the server end. This is used to look up the handlers for specific messages at specific network stages.
                    Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, std::pair<MessageConstructorPointer, MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer>>>> mServerStageMap;
                    //! The message mapping for the client end. This is used to look up the handlers for specific messages at specific network stages.
                    Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, std::pair<MessageConstructorPointer, MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient>>>> mClientStageMap;

                    //! The general message map mapping message type ID's to their constructors.
                    Support::UnorderedMap<Common::U32, MessageConstructorPointer> mMessageMap;
                    //! A mapping of datablock ID's to their constructors.
                    Support::UnorderedMap<Common::U32, NetworkDataBlockConstructorPointer> mDatablockTypeMap;
                    //! A mapping of datablock ID's to their constructors.
                    Support::UnorderedMap<Common::U32, NetworkEntityConstructorPointer> mEntityTypeMap;

                    /**
                     *  @brief Helper method to register all the known message types to the registry.
                     */
                    void registerMessages(void);

                    /**
                     *  @brief Helper method to register all the known datablock types to the registry.
                     */
                    void registerDatablockTypes(void);

                    /**
                     *  @brief Helper method to register all of the known entity types to the registry.
                     */
                    void registerEntityTypes(void);

                public:
                    Game::Entities::IEntity* constructEntity(const Common::U32 id, Support::CBitStream& payload);

                    /**
                     *  @brief Registers a networked message type to be instantiated indirectly across a network.
                     *  @param serverHandler Server side programming handler. If nullptr, then there is no serverside handler for this message type.
                     *  @param clientHandler Client side programming handler. If nullptr, then there is no clientside handler for this message type.
                     *  @param stage The stage at which this message type and handlers are valid at.
                     */
                    template <typename messageClass>
                    void registerMessage(MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer> serverHandler, MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient> clientHandler, const Net::STAGE_NAME stage)
                    {
                        MessageConstructorPointer messageConstructor = Net::IMessage::constructMessage<messageClass>;

                        assert(Net::IMessage::SharedStatics<messageClass>::sMessageID == -1);
                        Net::IMessage::SharedStatics<messageClass>::sMessageID = mMessageTypeCounter;

                        mMessageMap[mMessageTypeCounter] = messageConstructor;

                        if (serverHandler)
                            mServerStageMap[stage][mMessageTypeCounter] = std::make_pair(messageConstructor, serverHandler);

                        if (clientHandler)
                            mClientStageMap[stage][mMessageTypeCounter] = std::make_pair(messageConstructor, clientHandler);

                        ++mMessageTypeCounter;
                    }

                    /**
                     *  @brief Registers a networked datablock type to be instantiated indirectly across a network.
                     */
                    template <typename datablockClass>
                    void registerDataBlockType(void)
                    {
                        NetworkDataBlockConstructorPointer datablockConstructor = Game::Entities::DataBlocks::IDataBlock::constructNetworkDataBlock<datablockClass>;

                        assert(Game::Entities::DataBlocks::IDataBlock::SharedStatics<datablockClass>::sDataBlockID == -1);

                        Game::Entities::DataBlocks::IDataBlock::SharedStatics<datablockClass>::sDataBlockID = mDataBlockTypeCounter;
                        mDatablockTypeMap[mDataBlockTypeCounter] = datablockConstructor;

                        ++mDataBlockTypeCounter;
                    }

                    /**
                     *  @brief Registers a networked entity type to be instantiated indirectly across a network.
                     */
                    template <typename entityClass>
                    void registerEntityType(void)
                    {
                        NetworkEntityConstructorPointer entityConstructor = Game::Entities::IEntity::constructNetworkEntity<entityClass>;

                        assert(Game::Entities::IEntity::SharedStatics<entityClass>::sEntityTypeID == -1);

                        Game::Entities::IEntity::SharedStatics<entityClass>::sEntityTypeID  = mEntityTypeCounter;
                        mEntityTypeMap[mEntityTypeCounter] = entityConstructor;

                        ++mEntityTypeCounter;
                    }

                    /**
                     *  @brief Retrieves the type ID for the specified entity.
                     *  @return The type ID of the entity.
                     */
                    template <typename entityClass>
                    Common::S32 getEntityTypeID(void)
                    {
                        assert(Game::Entities::IEntity::SharedStatics<entityClass>::sEntityTypeID != -1);
                        return Game::Entities::IEntity::SharedStatics<entityClass>::sEntityTypeID;
                    }

                    /**
                     *  @brief Looks up a server message handler, returning the method pointer to the handler.
                     *  @param stage The stage at which we are trying to retrieve for.
                     *  @param id The ID of the message to lookup.
                     *  @return A pointer to the message handler to call. If no handler, nullptr is returned.
                     */
                    MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer> lookupServerMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id);

                    /**
                     *  @brief Looks up a client message handler, returning the method pointer to the handler.
                     *  @param stage The stage at which we are trying to retrieve for.
                     *  @param id The ID of the message to lookup.
                     *  @return A pointer to the message handler to call. If no handler, nullptr is returned.
                     */
                    MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient> lookupClientMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id);

                // Protected Methods
                protected:
                    //! Parameter-less constructor.
                    SCoreRegistry(void);
                    //! Standard destructor.
                    ~SCoreRegistry(void);
            };
        } // End NameSpace Core
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_
