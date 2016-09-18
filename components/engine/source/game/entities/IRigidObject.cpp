/**
 *  @file IRigidOBject.hpp
 *  @brief Include file defining IRigidOBject class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/entities/IRigidObject.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                IRigidObject::IRigidObject(const ENTITY_TYPE& type, const Game::Entities::EntityHintMask& hintMask) : Game::Entities::IEntity(type, hintMask)
                {
                }
            } // End NameSpace Entities
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
