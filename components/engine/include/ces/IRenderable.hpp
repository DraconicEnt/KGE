/**
 *  @file IRenderable.hpp
 *  @brief Include file declaring the IRenderable interface class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_IRENDERABLE_HPP_
#define _INCLUDE_IRENDERABLE_HPP_

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            /**
             *  @brief The IRenderable class is a base interface class for all renderable components to be used in a CES behavior model
             *  to derive from.
             */
            class IRenderable : public IComponent
            {
                // Public Methods
                public:
                    virtual COMPONENT_TYPE getComponentType(void) { return COMPONENT_RENDERABLE; }

                    virtual void attachComponent(IComponent* component)
                    {
                        switch (component->getComponentType())
                        {
                            case COMPONENT_ATTRIBUTE:
                            case COMPONENT_RENDERABLE:
                            case COMPONENT_PHYSICAL:
                                mChildren.push_back(component);
                                break;

                            default:
                                throw std::out_of_range("Attached component must be a renderable, attribute or physical component type!");
                        }
                    }
            };
        }
    }
}
#endif // _INCLUDE_IRENDERABLE_HPP_
