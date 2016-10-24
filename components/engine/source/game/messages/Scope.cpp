/**
 *  @file Scope.cpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <stdexcept>

#include <net/IIncomingClient.hpp>
#include <game/messages/Scope.hpp>

#include <net/INetworkPersistable.hpp>

#include <support/Console.hpp>

#include <game/entities/entities.hpp>

#include <core/SCoreRegistry.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Messages
            {
                Scope::Scope(Support::CBitStream* in, Net::IIncomingClient* sender) : IMessage(in, sender), mScopedCount(0)
                {
                }

                void Scope::add(const Net::INetworkPersistable* entity)
                {
                    mScoped.insert(mScoped.end(), entity);
                }

                void Scope::packEverything(Support::CBitStream& out) const
                {
                    IMessage::packBaseData<Scope>(out);

                    out << static_cast<Common::U32>(mScoped.size());

                    for (const Net::INetworkPersistable* currentPersistable : mScoped)
                        currentPersistable->packEverything(out);
                }

                void Scope::unpack(Support::CBitStream& in)
                {
                    if (in.getSize() - in.getPointer() < this->getMinimumPacketPayloadLength())
                        throw std::underflow_error("Unable to unpack Scope packet; too small of a payload!");

                    in >> mScopedCount;
                    CONSOLE_DEBUGF("Scope: Unpacking %u entities.", mScopedCount);

                    Core::SCoreRegistry* registry = Core::SCoreRegistry::getInstance();

                    for (Common::U32 iteration = 0; iteration < mScopedCount; ++iteration)
                    {
                        // Read off an ID and a type
                        Common::U32 netID = 0;
                        Common::U32 type = 0;
                        in >> type >> netID;

                        // Construct the entity
                        if (!registry->constructEntity(type, in))
                        {
                            Support::String message = "Scope: Invalid entity type to unpack: ";
                            message += type;
                            throw std::logic_error(message);
                            break;
                        }
                    }
                }

                size_t Scope::getMinimumPacketPayloadLength(void) const
                {
                    return (sizeof(Common::U32) * 1);
                }

                size_t Scope::getRequiredMemory(void) const
                {
                    return (sizeof(Common::U8) * 3) + (sizeof(Common::U32) * 2) + Net::IMessage::getRequiredMemory();
                }
            } // End NameSpace Messages
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
