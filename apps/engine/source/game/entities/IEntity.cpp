/**
 *  @file RigidProp.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#include <game/entities/IEntity.hpp>
#include <engine/SGameWorld.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            IEntity::IEntity(const Kiaro::Game::Entities::TypeMask &typeMask, const Kiaro::Game::Entities::EntityHintMask &hintMask) : mTypeMask(typeMask),
            mHintMask(hintMask), mNetID(0)
            {

            }

            IEntity::~IEntity(void)
            {

            }

            Kiaro::Common::U32 IEntity::getTypeMask(void) const { return mTypeMask; }

            Kiaro::Common::U32 IEntity::getNetID(void) const { return mNetID; }

            Kiaro::Common::U32 IEntity::getHintMask(void) const { return mHintMask; }

            void IEntity::packUpdate(Kiaro::Support::CBitStream &out)
            {

            }

            void IEntity::unpackUpdate(Kiaro::Support::CBitStream &in)
            {

            }

            void IEntity::packInitialization(Kiaro::Support::CBitStream &out)
            {

            }

            void IEntity::unpackInitialization(Kiaro::Support::CBitStream &in)
            {

            }

            void IEntity::instantiate(void)
            {
                Kiaro::Engine::SGameWorld::getPointer()->addEntity(this);
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
