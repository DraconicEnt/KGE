
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
                    CComponentRoot(void);

                    #ifdef _DEBUG
                    /**
                     *  @brief An internal validation rule useful for determining bugs that validate component rules at runtime and managed to
                     *  bypass the type checking in attachComponent.
                     */
                    virtual void internalValidate(std::set<IComponent*> trail);
                    #endif // _DEBUG

                    virtual void attachComponent(IComponent* component);

                    virtual COMPONENT_TYPE getComponentType(void);
            };
        }
    }
}
#endif // _INCLUDE_CCOMPONENTROT_HPP_
