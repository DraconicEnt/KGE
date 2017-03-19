/**
 *  @file CBuoyancy.cpp
 *  @brief Source file containing programming for the CBuoyancy class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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
