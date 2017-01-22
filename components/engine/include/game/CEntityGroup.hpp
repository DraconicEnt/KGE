/**
 *  @file CEntityGroup.cpp
 *  @brief Include file declaring the CEntityGroup class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_CENTITYGROUP_HPP_
#define _INCLUDE_GAME_CENTITYGROUP_HPP_

#include <support/common.hpp>
#include <support/UnorderedSet.hpp>

#include <game/entities/IEntity.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            class CEntityGroup : public IEngineObject
            {
                // Private Members
                private:
                    Support::UnorderedSet<Entities::IEntity*> mEntities;

                // Public Members
                public:
                    typedef Support::UnorderedSet<Entities::IEntity*>::iterator iterator;
                    typedef Support::UnorderedSet<Entities::IEntity*>::const_iterator const_iterator;

                // Public Methods
                public:
                    iterator begin(void);
                    const_iterator end(void);

                    void add(Entities::IEntity* entity);
                    void remove(Entities::IEntity* entity);
            };
        }
    }
}
#endif // _INCLUDE_GAME_CENTITYGROUP_HPP_
