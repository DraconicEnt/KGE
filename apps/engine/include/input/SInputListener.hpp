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

#include <support/common.hpp>
#include <irrlicht.h>

#include <CEGUI/CEGUI.h>
#include <physfs.h>

#include <support/UnorderedMap.hpp>

namespace Kiaro
{
    namespace Input
    {
        class SInputListener : public irr::IEventReceiver
        {
            // Public Members
            public:
                typedef void (*KeyResponderPointer)(bool state);

            // Private Members
            private:
                 bool mKeyStates[256];

                 Support::UnorderedMap<Common::C8, KeyResponderPointer> mKeyResponders;

            // Public Methods
            public:
                static SInputListener *getPointer(void);
                static void destroy(void);

                bool OnEvent(const irr::SEvent &event);

                void setKeyResponder(const CEGUI::Key::Scan& key, KeyResponderPointer responder);

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
