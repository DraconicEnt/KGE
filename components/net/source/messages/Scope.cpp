/**
 *  @file Scope.cpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <stdexcept>

#include <net/CIncomingClient.hpp>
#include <net/messages/Scope.hpp>

#include <net/INetworkPersistable.hpp>

#include <support/Logging.hpp>

namespace Kiaro
{
    namespace Net
    {
        namespace Messages
        {
            Scope::Scope(Support::CBitStream* in, CIncomingClient* sender) : IMessage(TYPE_SCOPE, in, sender), mScopedCount(0)
            {

            }

            void Scope::add(const INetworkPersistable* entity)
            {
                mScoped.insert(mScoped.end(), entity);
            }

            void Scope::packEverything(Support::CBitStream& out) const
            {
                for (auto it = mScoped.begin(); it != mScoped.end(); it++)
                    (*it)->packEverything(out);

                out << static_cast<Common::U32>(mScoped.size());

                IMessage::packEverything(out);
            }

            void Scope::unpack(Support::CBitStream& in)
            {
                if (in.getWrittenLength() < this->getMinimumPacketPayloadLength())
                    throw std::underflow_error("Unable to unpack Scope packet; too small of a payload!");

                in >> mScopedCount;

                Support::Logging::write(Support::Logging::MESSAGE_DEBUG, "Scope: Unpacking %u entities.", mScopedCount);

                for (Common::U32 iteration = 0; iteration < mScopedCount; iteration++)
                {
                            // Read off an ID and a type
                    Common::U32 netID = 0;
                    Common::U32 type = 0;

                    in >> netID >> type;

                    switch(type)
                    {
                                /*
                                case Game::Entities::ENTITY_TERRAIN:
                                {
                                    Game::Entities::CTerrain* terrain = new Game::Entities::CTerrain(in);
                                    terrain->setNetID(netID);
                                    Game::SGameWorld::getPointer()->addEntity(terrain);
                                    break;
                                }
                                */

                        default:
                        {
                                  //  throw std::logic_error("Scope: Invalid entitiy type to unpack!");
                            break;
                        }
                    }
                }
            }

            Common::U32 Scope::getMinimumPacketPayloadLength(void)
            {
                return (sizeof(Common::U32) * 2) + (sizeof(Common::U8) * 3);
            }

            size_t Scope::getRequiredMemory(void)
            {
                return (sizeof(Common::U8) * 3) + (sizeof(Common::U32) * 2) + IMessage::getRequiredMemory();
            }
        } // End NameSpace Messages
    } // End NameSpace Game
} // End NameSpace Kiaro