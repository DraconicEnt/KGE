/**
 */

#include <ces/CBuoyancy.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            CBuoyancy::CBuoyancy(float buoyancy) : mBuoyancy(buoyancy)
            {

            }

            void CBuoyancy::attachComponent(IComponent* component)
            {
                throw std::runtime_error("CBuoyancy components cannot have children!");
            }

            COMPONENT_TYPE CBuoyancy::getComponentType(void)
            {
                return COMPONENT_PHYSICAL_ATTRIBUTE;
            }
        }
    }
}
