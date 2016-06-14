/**
 *  @file CComponentGrabity.cpp
 *  @brief Source file implementing the CES::CComponentGravity class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <ces/CComponentGravity.hpp>

namespace Kiaro
{
    namespace CES
    {
        CComponentGravity::CComponentGravity(void)
        {
            mComponentTypeIdentifier = COMPONENT_GRAVITY;
        }

        void CComponentGravity::initialize(void)
        {
        }

        void CComponentGravity::deinitialize(void)
        {
        }
    }
}
