/**
 *  @file RigidObjectBase.hpp
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

#include <game/entities/RigidObjectBase.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            RigidObjectBase::RigidObjectBase(const Kiaro::Game::Entities::TypeMask &typeMask, const Kiaro::Game::Entities::EntityHintMask &hintMask) : Kiaro::Game::Entities::EntityBase(typeMask, hintMask)
            {

            }
        } // End NameSpace Entities
    } // End NameSpace Game
} // End NameSpace Kiaro
