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

#include <game/entities/RigidProp.hpp>
#include <game/entities/Types.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            RigidProp::RigidProp(void) : Kiaro::Game::Entities::RigidObjectBase(Kiaro::Game::Entities::ENTITY_RIGIDPROP)
            {

            }

            RigidProp::~RigidProp(void)
            {

            }

            void RigidProp::packUpdate(Kiaro::Support::BitStream &out)
            {

            }

            void RigidProp::unpackUpdate(Kiaro::Support::BitStream &in)
            {

            }

            void RigidProp::packInitialization(Kiaro::Support::BitStream &out)
            {

            }

            void RigidProp::unpackInitialization(Kiaro::Support::BitStream &in)
            {

            }

            void RigidProp::instantiate(void)
            {

            }

            void RigidProp::update(const Kiaro::Common::F32 &deltaTimeSeconds)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
