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

#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>
#include <game/SGameWorld.hpp>

#include <video/CSceneGraph.hpp>
#include <video/SRenderer.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                IEntity::IEntity(const ENTITY_TYPE& typeMask, const EntityHintMask& hintMask) : IEngineObject(), mType(typeMask),
                mFlags(hintMask), mNetID(0)
                {
                }

                IEntity::~IEntity(void)
                {
                }

                const Common::U32 IEntity::getTypeMask(void) const
                {
                    return mType;
                }

                Common::U32 IEntity::getNetID(void) const
                {
                    return mNetID;
                }

                Common::U32 IEntity::getTypeID(void) const
                {
                    return mType;
                }

                void IEntity::unpack(Support::CBitStream& in)
                {
                    Net::INetworkPersistable::unpack(in);
                }

                void IEntity::setNetID(const Common::U32 identifier)
                {
                    mNetID = identifier;
                }

                void IEntity::registerEntity(void)
                {
                    Game::SGameWorld::instantiate()->addEntity(this);

                    // Also register the entity with our current scene graph
                    Engine::Video::SRenderer::instantiate()->getCurrentScene()->add(this);
                }
            } // End Namespace Entities
        } // End Namespace Game
    }
} // End Namespace Kiaro
