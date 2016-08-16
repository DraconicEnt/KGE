/**
 *  @file Math.cpp
 *  @brief Source file implementing math helpers.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        Common::F32 FPSToMS(const Common::F32 fps)
        {
            return 1000.0f / fps;
        }
    }
}
