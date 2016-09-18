/**
 */

#ifndef _INCLUDE_CBUOYANCY_HPP_
#define _INCLUDE_CBUOYANCY_HPP_

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            class CBuoyancy : public IComponent
            {
                private:
                    Common::F32 mBuoyancy;

                public:
                    CBuoyancy(Common::F32 buoyancy);

                    virtual void attachComponent(IComponent* component);

                    virtual COMPONENT_TYPE getComponentType(void);
            };
        }
    }
}
#endif // _INCLUDE_CBUOYANCY_HPP_
