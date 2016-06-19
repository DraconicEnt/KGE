/**
 *  @file RigidProp.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
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

            const Common::U32 IEntity::getNetID(void) const
            {
                return mNetID;
            }

            void IEntity::packDeltas(Support::CBitStream& out)
            {
            }

            const ENTITY_TYPE& IEntity::getType(void) const
            {
                return mType;
            }

            void IEntity::packEverything(Support::CBitStream& out) const
            {
                // Here we pack our net ID and type ID
                out << mType << mNetID;
            }

            void IEntity::unpack(Support::CBitStream& in)
            {
                in >> mType >> mNetID;
            }

            void IEntity::setNetID(const Common::U32 identifier)
            {
                mNetID = identifier;
            }

            void IEntity::registerEntity(void)
            {
                Game::SGameWorld::getPointer()->addEntity(this);

                // Also register the entity with our current scene graph
                Engine::Video::SRenderer::getPointer()->getCurrentScene()->add(this);
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
