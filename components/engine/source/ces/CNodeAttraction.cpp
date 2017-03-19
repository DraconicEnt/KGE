/**
 *  @file CNodeAttraction.cpp
 *  @brief Source file containing programming for the CNodeAttraction class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/CNodeAttraction.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            COMPONENT_TYPE CNodeAttraction::getComponentType(void) { return COMPONENT_PHYSICAL_LOGIC; }

            void CNodeAttraction::attachComponent(IComponent* component)
            {
                throw std::runtime_error("CNodeAttraction components cannot have children!");
            }

            bool CNodeAttraction::evaluateComponent(const Support::UnorderedSet<CModelInstance*>& entities, Support::Set<IComponent*>& root)
            {
                // TODO: Here we'd have logic that would trigger the physical object to gravitate towards the specified top level renderable
                return true;
            }
        }
    }
}

