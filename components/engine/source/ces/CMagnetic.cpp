/**
 *  @file CMagnetic.cpp
 *  @brief Source file containing programming for the CMagnetic class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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
