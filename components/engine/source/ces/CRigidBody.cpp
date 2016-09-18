/**
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
                        mChildren.insert(mChildren.end(), component);
                        break;

                    default:
                        throw std::runtime_error("Attached component must be a physical attribute or physical logic!");
                }
            }
        }
    }
}
