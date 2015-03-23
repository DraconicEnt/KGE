/**
 *  @file CComponentGrabity.cpp
 *  @brief Source file implementating the Kiaro::CES::BehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <ces/CComponentGravity.hpp>

namespace Kiaro
{
    namespace CES
    {
        CComponentGrabity::CComponentGrabity(void)
        {
            mComponentTypeIdentifier = Kiaro::CES::COMPONENT_GRAVITY;
        }

        void CComponentGrabity::initialize(void)
        {

        }

        void CComponentGrabity::deinitialize(void)
        {

        }
    }
}
