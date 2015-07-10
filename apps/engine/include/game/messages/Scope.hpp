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

#ifndef _INCLUDE_GAME_MESSAGES_SCOPE_HPP_
#define _INCLUDE_GAME_MESSAGES_SCOPE_HPP_

#include <stdexcept>

#include <core/common.hpp>
#include <game/messages/messages.hpp>
#include <net/IMessage.hpp>
#include <game/entities/entities.hpp>

#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Network
    {
        class IncomingClient;
        class NetworkedEntity;
    } // End NameSpace Network

    namespace Game
    {
        namespace Messages
        {
            class Scope : public Net::IMessage
            {
                // Private Members
                private:
                    //! A Kiaro::Common::U32 representing the number of scoped entities in this message.
                    Common::U32 mScopedCount;

                    Support::UnorderedSet<const Entities::IEntity*> mScoped;

                // Public Methods
                public:
                    Scope(Support::CBitStream* in = NULL, Net::CClient* sender = NULL) : Net::IMessage(MESSAGE_SCOPE, in, sender), mScopedCount(0)
                    {

                    }

                    void add(const Entities::IEntity* entity)
                    {
                        mScoped.insert(mScoped.end(), entity);
                    }

                    void writeTo(Support::CBitStream& out) const
                    {
                        for (auto it = mScoped.begin(); it != mScoped.end(); it++)
                            (*it)->packInitialization(out);

                        out << static_cast<Common::U32>(mScoped.size());

                        Net::IMessage::writeTo(out);
                    }

                    void extractFrom(Support::CBitStream& in)
                    {
                        if (in.getWrittenLength() < this->getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack Scope packet; too small of a payload!");

                        in >> mScopedCount;

                        Core::Logging::write(Core::Logging::MESSAGE_DEBUG, "Scope: Unpacking %u entities.", mScopedCount);

                        for (Common::U32 iteration = 0; iteration < mScopedCount; iteration++)
                        {
                            // Read off an ID and a type
                            Common::U32 netID = 0;
                            Common::U32 type = 0;

                            in >> netID >> type;

                            switch(type)
                            {
                                case Game::Entities::ENTITY_TERRAIN:
                                {
                                    Game::Entities::CTerrain* terrain = new Game::Entities::CTerrain(in);
                                    terrain->setNetID(netID);
                                    Game::SGameWorld::getPointer()->addEntity(terrain);
                                    break;
                                }

                                default:
                                {
                                  //  throw std::logic_error("Scope: Invalid entitiy type to unpack!");
                                    break;
                                }
                            }
                        }
                    }

                    Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return (sizeof(Common::U32) * 2) + (sizeof(Common::U8) * 3);
                    }

                    size_t getRequiredMemory(void)
                    {
                        return (sizeof(Common::U8) * 3) + (sizeof(Common::U32) * 2) + Net::IMessage::getRequiredMemory();
                    }
            };
        } // End NameSpace Messages
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_MESSAGES_SCOPE_HPP_
