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

#include <game/entities/EntityBase.hpp>
#include <engine/EntityGroupingSingleton.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            EntityBase::EntityBase(const Kiaro::Game::Entities::TypeMask &typeMask, const Kiaro::Game::Entities::EntityHintMask &hintMask) : mTypeMask(typeMask),
            mHintMask(hintMask), mNetID(0)
            {

            }

            EntityBase::~EntityBase(void)
            {

            }

            Kiaro::Common::U32 EntityBase::getTypeMask(void) const { return mTypeMask; }

            Kiaro::Common::U32 EntityBase::getNetID(void) const { return mNetID; }

            Kiaro::Common::U32 EntityBase::getHintMask(void) const { return mHintMask; }

            void EntityBase::packUpdate(Kiaro::Support::BitStream &out)
            {

            }

            void EntityBase::unpackUpdate(Kiaro::Support::BitStream &in)
            {

            }

            void EntityBase::packInitialization(Kiaro::Support::BitStream &out)
            {

            }

            void EntityBase::unpackInitialization(Kiaro::Support::BitStream &in)
            {

            }

            void EntityBase::instantiate(void)
            {
                Kiaro::Engine::EntityGroupingSingleton::getPointer()->addEntity(this);
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
