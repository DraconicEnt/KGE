/**
 */

#ifndef _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_
#define _INCLUDE_KIARO_ENGINE_CORE_SCOREREGISTRY_HPP_

#include <game/SGameServer.hpp>
#include <core/COutgoingClient.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            class SCoreRegistry
            {
                public:
                    typedef EasyDelegate::DelegateSet<Net::IMessage*, Support::CBitStream&> MessageConstructorSet;
                    typedef EasyDelegate::DelegateSet<void, Net::IIncomingClient*, Support::CBitStream&> MessageHandlerSet;

                private:

                    Common::U32 mMessageCounter;

                    Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, std::pair<MessageConstructorSet::StaticDelegateFuncPtr, MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer>>>> mServerStageMap;
                    Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, std::pair<MessageConstructorSet::StaticDelegateFuncPtr, MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient>>>> mClientStageMap;
                    Support::UnorderedMap<Common::U32, MessageConstructorSet::StaticDelegateFuncPtr> mMessageMap;

                public:
                    static SCoreRegistry* getPointer(void);
                    static void destroy(void);

                    template <typename messageClass>
                    void registerMessage(MessageHandlerSet::MemberDelegateFuncPtr<Game::SGameServer> serverHandler, MessageHandlerSet::MemberDelegateFuncPtr<Core::COutgoingClient> clientHandler, const Net::STAGE_NAME stage)
                    {
                        MessageConstructorSet::StaticDelegateFuncPtr messageConstructor = Net::IMessage::constructMessage<messageClass>;

                        Net::IMessage::SharedStatics<messageClass>::sMessageID = mMessageCounter;

                        mMessageMap[mMessageCounter] = messageConstructor;

                        if (serverHandler)
                            mServerStageMap[stage][mMessageCounter] = std::make_pair(messageConstructor, serverHandler);

                        if (clientHandler)
                            mClientStageMap[stage][mMessageCounter] = std::make_pair(messageConstructor, clientHandler);

                        ++mMessageCounter;
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
