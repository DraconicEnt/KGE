/**
 *  @file CRigidBody.cpp
 *  @brief Source file containing programming for the CRigidBody class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/CRigidBody.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            COMPONENT_TYPE CRigidBody::getComponentType(void) { return COMPONENT_PHYSICAL; }

            CRigidBody::CRigidBody(const Support::String& filename)
            {
            }

            void CRigidBody::attachComponent(IComponent* component)
            {
                switch (component->getComponentType())
                {
                    case COMPONENT_PHYSICAL_LOGIC:
                    case COMPONENT_PHYSICAL_ATTRIBUTE:
                        IComponent::attachComponent(component);
                        break;

                    default:
                        throw std::runtime_error("Attached component must be a physical attribute or physical logic!");
                }
            }
        }
    }
}
