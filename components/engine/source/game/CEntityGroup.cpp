/**
 *  @file CEntityGroup.cpp
 */

#include <game/CEntityGroup.hpp>

namespace Kiaro
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

        void CEntityGroup::add(Entities::IEntity* entity)
        {
            mEntities.insert(mEntities.end(), entity);
        }

        void CEntityGroup::remove(Entities::IEntity* entity)
        {
            mEntities.erase(entity);
        }
    }
}
