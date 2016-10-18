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
#include <support/String.hpp>

namespace Kiaro
{
    namespace Input
    {
        /**
         *  @brief The SInputListener is the central class used for processing input events from the users physically at the machine
         *  running the game. It supports bind schemas as well as binding input events to script functions.
         */
        class SInputListener : public Support::ISingleton<SInputListener>
        {
            // Public Members
            public:
                typedef EasyDelegate::ITypedDelegate<void, ALLEGRO_EVENT&> InputEventResponderDelegate;

                //! An enumeration used to identify input device types when binding input responders.
                enum INPUT_DEVICE
                {
                    //! NULL device.
                    INPUT_NULL = 0,

                    //! Mouse.
                    INPUT_MOUSE = 1,

                    //! Keyboard.
                    INPUT_KEYBOARD = 2,

                    //! Gamepad/controller/joystick/etc
                    INPUT_GAMEPAD = 3,
                };

                //! A struct representing a binary input responder.
                struct InputResponder
                {
                    //! The input device type.
                    INPUT_DEVICE mInputDevice;

                    //! The responder to call.
                    InputEventResponderDelegate* mInputResponder;
                };

                //! An event set type for joystick configuration changes, Ie: Controllers plugged in or unplugged.
                typedef EasyDelegate::DelegateSet<void, ALLEGRO_JOYSTICK*, bool> JoystickConfigEventType;

                //! A pointer to a method that will repsond toa  key press.
                typedef void (*KeyResponderPointer)(const bool state);

                //! A boolean representing whether or not keyboard events should be forwarded to CEGUI (typing) or processed as keybind invocations.
                bool mTyping;

                //! An event set for joystick configuration changes, Ie: Controllers plugged in or unplugged.
                JoystickConfigEventType mJoystickConfigEventSet;

            // Private Members
            private:
                //! An unordered map mapping input scheme names to an unordered map mapping INPUT_DEVICE types to yet another unordered map mapping device ID's to ANOTHER unordered map mapping input codes to their appropriate responders.
                Support::UnorderedMap<Support::String, Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, Support::UnorderedMap<Common::U32, InputEventResponderDelegate*>>>> mInputSchemes;

                //! All joysticks currently attached to the machine.
                Support::UnorderedSet<ALLEGRO_JOYSTICK*> mAttachedJoysticks;

                //! The input queue that input events will be processed out of.
                ALLEGRO_EVENT_QUEUE* mInputQueue;

                //! A mapping of Allegro joystick pointers to joystick identifiers.
                Support::UnorderedMap<ALLEGRO_JOYSTICK*, Common::U32> mJoystickIDs;

                //! The name of the current scheme in use.
                Support::String mInputScheme;

            // Public Methods
            public:
                //! Updating the input subsystem allows for it to process keyboard, joystick and other user input events.
                void update(void);

                /**
                 *  @brief Sets an input responder.
                 *  @param scheme The name of the input scheme to bind the responder in.
                 *  @param inputCode The inputCode to bind to. What exactly this should be varies depending on the deviceType. For example, controllers have their codes indexed 0 through buttonCount
                 *  for all buttons and then buttonCount + 1 through buttonCount + stickCount are indexed for joysticks and other analog inputs.
                 *  @param deviceType The type of device we want to bind the responder to.
                 *  @param responder The pointer to the delegate representing our responder.
                 */
                void setResponder(const Support::String& scheme, const Common::U32 inputCode, const INPUT_DEVICE deviceType, const Common::U32 deviceID, InputEventResponderDelegate* responder);

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

                /**
                 *  @brief Tells the SInputListener to use a different input scheme when dealing with input events.
                 *  @param name The name of the scheme to use.
                 *  @return True if the new scheme was found and used successfully. False otherwise.
                 */
                bool setInputScheme(const Support::String& name);

                /**
                 *  @brief Returns the name of the input scheme currently in use.
                 *  @return A const reference to the input scheme currently in use.
                 */
                const Support::String& getInputScheme(void);

            // Private Methods
            private:
                /**
                 *  @brief Helper method to dispatch input responses based on the current input scheme in use.
                 *  @param inputCode The inputCode to bind to. What exactly this should be varies depending on the deviceType. For example, controllers have their codes indexed 0 through buttonCount
                 *  for all buttons and then buttonCount + 1 through buttonCount + stickCount are indexed for joysticks and other analog inputs.
                 *  @param deviceType The type of device we want to bind the responder to.
                 *  @param event A reference to the input event created by allegro.
                 */
                void dispatchInputResponse(const Common::U32 inputCode, const INPUT_DEVICE deviceType, const Common::U32 deviceID, ALLEGRO_EVENT& event);

            // Protected Methods
            protected:
                //! Parameter-less constructor.
                SInputListener(void);

                //! Standard destructor.
                ~SInputListener(void);
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_SINPUTLISTENERSINGLETON_HPP_
