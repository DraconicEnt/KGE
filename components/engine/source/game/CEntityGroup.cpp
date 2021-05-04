/**
 *  @file CEntityGroup.cpp
 *  @brief Source file implementing CEntityGroup class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/CEntityGroup.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            CEntityGroup::iterator CEntityGroup::begin(void)
            {
                return mEntities.begin();
            }

            CEntityGroup::const_iterator CEntityGroup::end(void)
            {
                return mEntities.end();
            }

            void CEntityGroup::add(IEntity* entity)
            {
                mEntities.insert(mEntities.end(), entity);
            }

            void CEntityGroup::remove(IEntity* entity)
            {
                mEntities.erase(entity);
            }
        }
    }
}
