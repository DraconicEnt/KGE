/**
 */

#include <iostream>

#include <ces/CMagnetic.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            CMagnetic::CMagnetic(float strength) : mMagnetism(strength)
            {

            }

            void CMagnetic::attachComponent(IComponent* component)
            {
                throw std::runtime_error("CMagnetic components cannot have children!");
            }

            COMPONENT_TYPE CMagnetic::getComponentType(void)
            {
                return COMPONENT_PHYSICAL_ATTRIBUTE;
            }
        }
    }
}
