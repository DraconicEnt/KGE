/**
 *  @file SInputListener.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_
#define _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_

#include <support/common.hpp>
#include <irrlicht.h>

#include <CEGUI/CEGUI.h>
#include <physfs.h>

#include <support/UnorderedMap.hpp>
#include <easydelegate/easydelegate.hpp>
#include <support/ISingleton.hpp>

namespace Kiaro
{
    namespace Input
    {
        class SInputListener : public Support::ISingleton<SInputListener>
        {
                // Public Members
            public:
                typedef EasyDelegate::ITypedDelegate<void, bool> KeyResponderDelegate;

                typedef void (*KeyResponderPointer)(const bool state);

                //! A boolean representing whether or not keyboard events should be forwarded to CEGUI (typing) or processed as keybind invocations.
                bool mTyping;

                // Private Members
            private:
                //! A mapping of keycodes to responding method pointers.
                Support::UnorderedMap<Common::C8, KeyResponderDelegate*> mKeyResponders;

                ALLEGRO_EVENT_QUEUE* mInputQueue;

                // Public Methods
            public:
                SInputListener(void);
                ~SInputListener(void);

                //! Updating the input subsystem allows for it to process keyboard, joystick and other user input events.
                void update(void);

                void setKeyResponder(const CEGUI::Key::Scan key, KeyResponderDelegate* responder);

                void scanJoysticks(void);

                /**
                 *  @brief Capture or free the mouse from the active game window, if there is one.
                 *  @param enabled A boolean representing whether or not mouse capture should be
                 *  enabled.
                 *  @note Does nothing if there is no active game window -- IE: Dedicated servers.
                 */
                void setMouseCaptureEnabled(const bool enabled);
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_
