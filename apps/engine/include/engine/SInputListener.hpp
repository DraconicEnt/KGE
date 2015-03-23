/**
 *  @file SInputListener.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_
#define _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_

#include <engine/Common.hpp>
#include <irrlicht.h>

#include <physfs.h>

namespace Kiaro
{
    namespace Engine
    {
        class SInputListener : public irr::IEventReceiver
        {
            // Public Methods
            public:
                static SInputListener *getPointer(void);
                static void destroy(void);

                bool OnEvent(const irr::SEvent &event);

            // Private Methods
            private:
                //! Standard constructor.
                SInputListener(void);
                //! Standard destructor.
                ~SInputListener(void);
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_
