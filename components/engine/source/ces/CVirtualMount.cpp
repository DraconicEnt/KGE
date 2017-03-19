/**
 *  @file CVirtualMount.cpp
 *  @brief Source file containing programming for the CComponentRoot class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/CVirtualMount.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            COMPONENT_TYPE CVirtualMount::getComponentType(void)
            {
                return COMPONENT_ATTRIBUTE;
            }
        }
    }
}
