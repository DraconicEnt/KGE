#ifndef _INCLUDE_IRENDERABLE_HPP_
#define _INCLUDE_IRENDERABLE_HPP_

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            class IRenderable : public IComponent
            {
                private:

                public:
                    virtual COMPONENT_TYPE getComponentType(void) { return COMPONENT_RENDERABLE; }

                    virtual void attachComponent(IComponent* component)
                    {
                        switch (component->getComponentType())
                        {
                            case COMPONENT_ATTRIBUTE:
                            case COMPONENT_RENDERABLE:
                            case COMPONENT_PHYSICAL:
                                mChildren.insert(mChildren.end(), component);
                                break;

                            default:
                                throw std::out_of_range("Attached component must be a renderable, attribute or physical component type!");
                        }

                        mChildren.insert(mChildren.end(), component);
                    }
            };
        }
    }
}
#endif // _INCLUDE_IRENDERABLE_HPP_
