/**
 *  @file CRigidObject.hpp
 *  @brief Include file defining RigidObjectBase class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_RIGIDOBJECTBASE_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_RIGIDOBJECTBASE_HPP_

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

#include <game/entities/types.hpp>
#include <game/entities/IEntity.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            class IRigidObject : public IEntity
            {
                // Protected Members
                protected:
                    irr::scene::ISceneNode* mRender;
                    btRigidBody* mPhysicalBody;

                // Public Methods
                public:
                    IRigidObject(const ENTITY_TYPE& type, const EntityHintMask& hintMask = 0);
            }; // End Class RigidObjectBase
        } // End NameSpace Entities
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_RIGIDOBJECTBASE_HPP_
