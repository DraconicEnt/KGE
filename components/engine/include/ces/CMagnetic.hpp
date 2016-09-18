/**
 */

#ifndef _INCLUDE_CES_CMAGNETIC_
#define _INCLUDE_CES_CMAGNETIC_

#include <support/common.hpp>

#include <ces/IComponent.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            class CMagnetic : public IComponent
            {
                private:
                    Common::F32 mMagnetism;

                public:
                    CMagnetic(Common::F32 strength);

                    void attachComponent(IComponent* component);

                    COMPONENT_TYPE getComponentType(void);
            };
        }
    }
}
#endif // _INCLUDE_CES_CMAGNETIC_
