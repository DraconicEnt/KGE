/**
 *  @file Scope.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_NET_MESSAGES_SCOPE_HPP_
#define _INCLUDE_NET_MESSAGES_SCOPE_HPP_

#include <support/common.hpp>
#include <net/messages/types.hpp>
#include <net/messages/IMessage.hpp>

#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CIncomingClient;
        class INetworkPersistable;

        namespace Messages
        {
            /**
             *  @brief A message type used to scope entities to specific clients. It basically notifies players
             *  that a specific thing exists and that they should create the clientside representation of it
             *  given the parameters packed in the message payload.
             */
            class Scope : public IMessage
            {
                // Private Members
                private:
                    //! A Common::U32 representing the number of scoped entities in this message.
                    Common::U32 mScopedCount;

                    //! A Support::UnorderedSet of all entities to pack.
                    Support::UnorderedSet<const Net::INetworkPersistable*> mScoped;

                // Public Methods
                public:
                    Scope(Support::CBitStream* in = NULL, Net::CIncomingClient* sender = NULL);
                    void add(const Net::INetworkPersistable* entity);

                    virtual void packEverything(Support::CBitStream& out) const;

                    virtual void unpack(Support::CBitStream& in);

                    Common::U32 getMinimumPacketPayloadLength(void);

                    size_t getRequiredMemory(void);
            };
        } // End NameSpace Messages
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_NET_MESSAGES_SCOPE_HPP_
