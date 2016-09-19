/**
 */

#ifndef _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_
#define _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_

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
            class SCoreRegistry
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

                    void registerEntityTypes(void);

                public:
                    static SCoreRegistry* getPointer(void);
                    static void destroy(void);

                    Game::Entities::IEntity* constructEntity(const Common::U32 id, Support::CBitStream& payload);

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

                    template <typename datablockClass>
                    void registerDataBlockType(void)
                    {
                        NetworkDataBlockConstructorPointer datablockConstructor = Game::Entities::DataBlocks::IDataBlock::constructNetworkDataBlock<datablockClass>;

                        assert(Game::Entities::DataBlocks::IDataBlock::SharedStatics<datablockClass>::sDataBlockID == -1);

                        Game::Entities::DataBlocks::IDataBlock::SharedStatics<datablockClass>::sDataBlockID = mDataBlockTypeCounter;
                        mDatablockTypeMap[mDataBlockTypeCounter] = datablockConstructor;

                        ++mDataBlockTypeCounter;
                    }

                    template <typename entityClass>
                    void registerEntityType(void)
                    {
                        NetworkEntityConstructorPointer entityConstructor = Game::Entities::IEntity::constructNetworkEntity<entityClass>;

                        assert(Game::Entities::IEntity::SharedStatics<entityClass>::sEntityTypeID == -1);

                        Game::Entities::IEntity::SharedStatics<entityClass>::sEntityTypeID  = mEntityTypeCounter;
                        mEntityTypeMap[mEntityTypeCounter] = entityConstructor;

                        ++mEntityTypeCounter;
                    }

                    MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer> lookupServerMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id);
                    MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient> lookupClientMessageHandler(const Net::STAGE_NAME stage, const Common::U32 id);

                private:
                    ~SCoreRegistry(void);
                    SCoreRegistry(void);
            };
        } // End NameSpace Core
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_
