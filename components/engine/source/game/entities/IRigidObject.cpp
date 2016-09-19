/**
 *  @file IRigidOBject.hpp
 *  @brief Include file defining IRigidOBject class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
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

                void IRigidObject::registerNetworkedProperties(void)
                {
                    this->addNetworkedProperty("position", mPosition);
                }
            } // End NameSpace Entities
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
