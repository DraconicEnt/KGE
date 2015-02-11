/**
 *  @file ComponentGravity.cpp
 *  @brief Source file implementating the Kiaro::CES::BehaviorModel class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <ces/ComponentGravity.hpp>

namespace Kiaro
{
    namespace CES
    {
        ComponentGravity::ComponentGravity(void)
        {
            mComponentTypeIdentifier = Kiaro::CES::COMPONENT_GRAVITY;
        }

        void ComponentGravity::initialize(void)
        {

        }

        void ComponentGravity::deinitialize(void)
        {

        }
    }
}
