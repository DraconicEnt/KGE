
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
                    if (component->getComponentType() != COMPONENT_RENDERABLE)
                        throw std::runtime_error("Attached component must be a renderable!");

                IComponent::internalValidate(trail);
            }
            #endif // _DEBUG

            void CComponentRoot::attachComponent(IComponent* component)
            {
                if (component->getComponentType() != COMPONENT_RENDERABLE)
                    throw std::out_of_range("Attached component must be a renderable!");

                mChildren.insert(mChildren.end(), component);

                IComponent::attachComponent(component);
            }

            COMPONENT_TYPE CComponentRoot::getComponentType(void) { return COMPONENT_ROOT; }
        }
    }
}
