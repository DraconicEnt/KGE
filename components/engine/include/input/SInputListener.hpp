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
#include <easydelegate/easydelegate.hpp>

namespace Kiaro
{
    namespace Input
    {
        class SInputListener
        {
            // Public Members
            public:
                typedef EasyDelegate::ITypedDelegate<void, bool> KeyResponderDelegate;
                
                typedef void (*KeyResponderPointer)(const bool& state);
                
                //! A boolean representing whether or not keyboard events should be forwarded to CEGUI (typing) or processed as keybind invocations.
                bool mTyping;

            // Private Members
            private:
                //! A mapping of keycodes to responding method pointers.
                Support::UnorderedMap<Common::C8, KeyResponderDelegate*> mKeyResponders;
                
                ALLEGRO_EVENT_QUEUE* mInputQueue;

            // Public Methods
            public:
                static SInputListener *getPointer(void);
                static void destroy(void);

                //! Updating the input subsystem allows for it to process keyboard, joystick and other user input events.
                void update(void);

                void setKeyResponder(const CEGUI::Key::Scan& key, KeyResponderDelegate* responder);
                
                void scanJoysticks(void);
                
                /**
                 *  @brief Capture or free the mouse from the active game window, if there is one.
                 *  @param enabled A boolean representing whether or not mouse capture should be
                 *  enabled.
                 *  @note Does nothing if there is no active game window -- IE: Dedicated servers.
                 */
                void setMouseCaptureEnabled(const bool& enabled);

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
