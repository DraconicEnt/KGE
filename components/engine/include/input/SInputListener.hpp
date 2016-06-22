/**
 *  @file SInputListener.hpp
 *  @brief Include file declaring the SInputListener singleton class.
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
        /*
         *
         */
        class SInputListener : public Support::ISingleton<SInputListener>
        {
            // Public Members
            public:
                //! A delegate representing a responder that has a range of states.
                typedef EasyDelegate::ITypedDelegate<void, Common::F32> AnalogResponderDelegate;

                //! A delegate representing a responder that has a range of states across two axiis.
                typedef EasyDelegate::ITypedDelegate<void, Common::F32, Common::F32> DualAnalogResponderDelegate;

                //! An event set type for joystick configuration changes, Ie: Controllers plugged in or unplugged.
                typedef EasyDelegate::DelegateSet<void, ALLEGRO_JOYSTICK*, bool> JoystickConfigEventType;

                typedef void (*KeyResponderPointer)(const bool state);

                //! A boolean representing whether or not keyboard events should be forwarded to CEGUI (typing) or processed as keybind invocations.
                bool mTyping;

                //! An event set for joystick configuration changes, Ie: Controllers plugged in or unplugged.
                JoystickConfigEventType mJoystickConfigEventSet;

            // Private Members
            private:
                //! A mapping of keycodes to responding method pointers.
                Support::UnorderedMap<Common::C8, AnalogResponderDelegate*> mKeyResponders;

                //! A map of allegro joysticks to a map of button ID's to an associated button responder.
                Support::UnorderedMap<ALLEGRO_JOYSTICK*, Support::UnorderedMap<Common::U32, AnalogResponderDelegate*>> mButtonResponders;

                Support::UnorderedSet<ALLEGRO_JOYSTICK*> mAttachedJoysticks;

                ALLEGRO_EVENT_QUEUE* mInputQueue;

            // Public Methods
            public:
                //! Parameter-less constructor.
                SInputListener(void);
                //! Standard destructor.
                ~SInputListener(void);

                //! Updating the input subsystem allows for it to process keyboard, joystick and other user input events.
                void update(void);

                void setKeyResponder(const CEGUI::Key::Scan key, AnalogResponderDelegate* responder);

                /**
                 * @brief Scans for joysticks connected to the system.
                 * @details This includes not just joysticks in the traditional sense, but also gamepads. This
                 * method is used only to populate the gamepad list for the SInputManager to deal with.
                 */
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
