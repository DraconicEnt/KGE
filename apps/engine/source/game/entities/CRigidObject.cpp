/**
 *  @file CRigidObject.cpp
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

#include <game/entities/CRigidObject.hpp>
#include <game/entities/types.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            CRigidObject::CRigidObject(void) : Game::Entities::IRigidObject(Game::Entities::ENTITY_RIGIDPROP)
            {

            }

            CRigidObject::~CRigidObject(void)
            {

            }

            void CRigidObject::packUpdate(Support::CBitStream &out)
            {

            }

            void CRigidObject::unpackUpdate(Support::CBitStream &in)
            {

            }

            void CRigidObject::packInitialization(Support::CBitStream &out)
            {

            }

            void CRigidObject::unpackInitialization(Support::CBitStream &in)
            {

            }

            void CRigidObject::instantiate(void)
            {

            }

            void CRigidObject::update(const Common::F32 &deltaTimeSeconds)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
