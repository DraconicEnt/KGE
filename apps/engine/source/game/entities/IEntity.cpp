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

#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>
#include <game/SGameWorld.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            IEntity::IEntity(const ENTITY_TYPE& typeMask, const EntityHintMask& hintMask) : mType(typeMask),
            mHintMask(hintMask), mNetID(0)
            {

            }

            IEntity::~IEntity(void)
            {

            }

            Common::U32 IEntity::getTypeMask(void) const { return mType; }

            Common::U32 IEntity::getNetID(void) const { return mNetID; }

            Common::U32 IEntity::getHintMask(void) const { return mHintMask; }

            void IEntity::packUpdate(Support::CBitStream& out)
            {

            }

            void IEntity::unpackUpdate(Support::CBitStream& in)
            {

            }

            void IEntity::packInitialization(Support::CBitStream& out) const
            {
                // Here we pack our net ID and type ID
                out << mType << mNetID;
            }

            void IEntity::unpackInitialization(Support::CBitStream &in)
            {
                in >> mNetID >> mType;
            }

            void IEntity::setNetID(const Common::U32& identifier)
            {
                mNetID = identifier;
            }

            void IEntity::instantiate(void)
            {
                Game::SGameWorld::getPointer()->addEntity(this);
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
