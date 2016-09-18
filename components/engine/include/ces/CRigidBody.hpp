/**
 */

#ifndef _INCLUDE_CRIGIDBODY_HPP_
#define _INCLUDE_CRIGIDBODY_HPP_

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            class CRigidBody : public IComponent
            {
                private:

                public:
                    CRigidBody(const Support::String& filename);

                    virtual COMPONENT_TYPE getComponentType(void);

                    virtual void attachComponent(IComponent* component);
            };
        }
    }
}
#endif // _INCLUDE_CRIGIDBODY_HPP_
