/**
 *  @file CComponentRoot.cpp
 *  @brief Source file containing programming for the CComponentRoot class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/CComponentRoot.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            CComponentRoot::CComponentRoot(void) : IComponent(this), mOriginIndex(-1)
            {

            }

            #ifdef _DEBUG
            void CComponentRoot::internalValidate(std::set<IComponent*> trail)
            {
                for (IComponent* component: mChildren)
                {
                    if (component->getComponentType() != COMPONENT_RENDERABLE)
                    {
                        throw std::runtime_error("Attached component must be a renderable!");
                    }
                }

                IComponent::internalValidate(trail);
            }
            #endif // _DEBUG

            void CComponentRoot::attachComponent(IComponent* component)
            {
                if (component->getComponentType() != COMPONENT_RENDERABLE)
                {
                    throw std::out_of_range("Attached component must be a renderable!");
                }

                IComponent::attachComponent(component);
            }

            COMPONENT_TYPE CComponentRoot::getComponentType(void) { return COMPONENT_ROOT; }
        }
    }
}
