/**
 *  @file CComponentRoot.hpp
 *  @brief Include file declaring the CComponentRoot class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CCOMPONENTROT_HPP_
#define _INCLUDE_CCOMPONENTROT_HPP_

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            /**
             *  @brief A component root can only have renderables attached to it. It is used to represent the root of a model.
             */
            class CComponentRoot : public IComponent
            {
                public:
                    //! The top level renderable index to consider as the entity's origin. If <0 then the initial spawn point is origin.
                    int mOriginIndex;

                public:
                    //! Parameter-less constructor.
                    CComponentRoot(void);

                    #ifdef _DEBUG
                    /**
                     *  @brief An internal validation rule useful for determining bugs that validate component rules at runtime and managed to
                     *  bypass the type checking in attachComponent.
                     */
                    virtual void internalValidate(std::set<IComponent*> trail);
                    #endif // _DEBUG

                    /**
                     *  @brief Attaches a component at the root level of the model and validates that this attachment is valid.
                     *  @param component The component to attach.
                     *  @throw std::out_of_range When the attachment is invalid. Ie: That type of component cannot attach here.
                     */
                    virtual void attachComponent(IComponent* component);

                    /**
                     *  @brief Gets the component type of this component.
                     *  @return The component type of this component.
                     */
                    virtual COMPONENT_TYPE getComponentType(void);
            };
        }
    }
}
#endif // _INCLUDE_CCOMPONENTROT_HPP_
