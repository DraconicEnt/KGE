/**
 *  @file SInputListener.cpp
 *  @brief Source file implementing the SInputListener singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <allegro5/allegro.h>

#include <input/SInputListener.hpp>

#include <video/SRenderer.hpp>
#include <support/Console.hpp>

#include <support/SProfiler.hpp>
#include <gui/SGUIManager.hpp>

namespace Kiaro
{
    namespace Input
    {
        SInputListener::SInputListener(void) : mTyping(false), mInputQueue(al_create_event_queue()), mInputScheme("")
        {
            al_install_mouse();
            al_install_keyboard();
            al_install_joystick();

            al_register_event_source(mInputQueue, al_get_keyboard_event_source());
            al_register_event_source(mInputQueue, al_get_joystick_event_source());
            al_register_event_source(mInputQueue, al_get_mouse_event_source());

            CONSOLE_INFO("Initialized input subsystem.");

            // Perform the initial joystick scan
            this->scanJoysticks();
        }

        SInputListener::~SInputListener(void)
        {
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_uninstall_joystick();
            al_destroy_event_queue(mInputQueue);

            CONSOLE_INFO("Destroyed input subsystem.");
        }

        static inline CEGUI::Key::Scan AllegroKeyToCEGUI(const Common::U32 code)
        {
            switch(code)
            {
                default:
                    CONSOLE_DEBUGF("Received unknown Allegro key code: %u", code);
                    break;

                // Ignored keycodes
                case ALLEGRO_KEY_PRINTSCREEN:
                    break;

                case ALLEGRO_KEY_A:
                    return CEGUI::Key::Scan::A;

                case ALLEGRO_KEY_B:
                    return CEGUI::Key::Scan::B;

                case ALLEGRO_KEY_C:
                    return CEGUI::Key::Scan::C;

                case ALLEGRO_KEY_D:
                    return CEGUI::Key::Scan::D;

                case ALLEGRO_KEY_E:
                    return CEGUI::Key::Scan::E;

                case ALLEGRO_KEY_F:
                    return CEGUI::Key::Scan::F;

                case ALLEGRO_KEY_G:
                    return CEGUI::Key::Scan::G;

                case ALLEGRO_KEY_H:
                    return CEGUI::Key::Scan::H;

                case ALLEGRO_KEY_I:
                    return CEGUI::Key::Scan::I;

                case ALLEGRO_KEY_J:
                    return CEGUI::Key::Scan::J;

                case ALLEGRO_KEY_K:
                    return CEGUI::Key::Scan::K;

                case ALLEGRO_KEY_L:
                    return CEGUI::Key::Scan::L;

                case ALLEGRO_KEY_M:
                    return CEGUI::Key::Scan::M;

                case ALLEGRO_KEY_N:
                    return CEGUI::Key::Scan::N;

                case ALLEGRO_KEY_O:
                    return CEGUI::Key::Scan::O;

                case ALLEGRO_KEY_P:
                    return CEGUI::Key::Scan::P;

                case ALLEGRO_KEY_Q:
                    return CEGUI::Key::Scan::Q;

                case ALLEGRO_KEY_R:
                    return CEGUI::Key::Scan::R;

                case ALLEGRO_KEY_S:
                    return CEGUI::Key::Scan::S;

                case ALLEGRO_KEY_T:
                    return CEGUI::Key::Scan::T;

                case ALLEGRO_KEY_U:
                    return CEGUI::Key::Scan::U;

                case ALLEGRO_KEY_V:
                    return CEGUI::Key::Scan::V;

                case ALLEGRO_KEY_W:
                    return CEGUI::Key::Scan::W;

                case ALLEGRO_KEY_X:
                    return CEGUI::Key::Scan::X;

                case ALLEGRO_KEY_Y:
                    return CEGUI::Key::Scan::Y;

                case ALLEGRO_KEY_Z:
                    return CEGUI::Key::Scan::Z;

                case ALLEGRO_KEY_0:
                    return CEGUI::Key::Scan::Zero;

                case ALLEGRO_KEY_1:
                    return CEGUI::Key::Scan::One;

                case ALLEGRO_KEY_2:
                    return CEGUI::Key::Scan::Two;

                case ALLEGRO_KEY_3:
                    return CEGUI::Key::Scan::Three;

                case ALLEGRO_KEY_4:
                    return CEGUI::Key::Scan::Four;

                case ALLEGRO_KEY_5:
                    return CEGUI::Key::Scan::Five;

                case ALLEGRO_KEY_6:
                    return CEGUI::Key::Scan::Six;

                case ALLEGRO_KEY_7:
                    return CEGUI::Key::Scan::Seven;

                case ALLEGRO_KEY_8:
                    return CEGUI::Key::Scan::Eight;

                case ALLEGRO_KEY_9:
                    return CEGUI::Key::Scan::Nine;

                case ALLEGRO_KEY_MINUS:
                    return CEGUI::Key::Scan::Minus;

                case ALLEGRO_KEY_TAB:
                    return CEGUI::Key::Scan::Tab;

                case ALLEGRO_KEY_ESCAPE:
                    return CEGUI::Key::Scan::Escape;

                case ALLEGRO_KEY_F1:
                    return CEGUI::Key::Scan::F1;

                case ALLEGRO_KEY_F2:
                    return CEGUI::Key::Scan::F2;

                case ALLEGRO_KEY_F3:
                    return CEGUI::Key::Scan::F3;

                case ALLEGRO_KEY_F4:
                    return CEGUI::Key::Scan::F4;

                case ALLEGRO_KEY_F5:
                    return CEGUI::Key::Scan::F5;

                case ALLEGRO_KEY_F6:
                    return CEGUI::Key::Scan::F6;

                case ALLEGRO_KEY_F7:
                    return CEGUI::Key::Scan::F7;

                case ALLEGRO_KEY_F8:
                    return CEGUI::Key::Scan::F8;

                case ALLEGRO_KEY_F9:
                    return CEGUI::Key::Scan::F9;

                case ALLEGRO_KEY_F10:
                    return CEGUI::Key::Scan::F10;

                case ALLEGRO_KEY_F11:
                    return CEGUI::Key::Scan::F11;

                case ALLEGRO_KEY_F12:
                    return CEGUI::Key::Scan::F12;

                case ALLEGRO_KEY_UP:
                    return CEGUI::Key::Scan::ArrowUp;

                case ALLEGRO_KEY_DOWN:
                    return CEGUI::Key::Scan::ArrowDown;

                case ALLEGRO_KEY_LEFT:
                    return CEGUI::Key::Scan::ArrowLeft;

                case ALLEGRO_KEY_RIGHT:
                    return CEGUI::Key::Scan::ArrowRight;

                case ALLEGRO_KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
            }

            return CEGUI::Key::Scan::Unknown;
        }

        void SInputListener::scanJoysticks(void)
        {
            mJoystickIDs.clear();

            // Check for any joysticks that were removed
            if (al_reconfigure_joysticks())
            {
                CONSOLE_INFO("Joystick configuration changed.");

                Support::UnorderedSet<ALLEGRO_JOYSTICK*> removedJoysticks;
                for (ALLEGRO_JOYSTICK* joystick: mAttachedJoysticks)
                    if (!al_get_joystick_active(joystick))
                    {
                        CONSOLE_INFOF("Detected removed joystick: %s", al_get_joystick_name(joystick));
                        removedJoysticks.insert(removedJoysticks.end(), joystick);

                        // Emit the event that the joystick was removed
                        mJoystickConfigEventSet.invoke(joystick, false);
                    }

                // Now delete the removed joysticks
                for (ALLEGRO_JOYSTICK* removedJoystick: removedJoysticks)
                {
                    mAttachedJoysticks.erase(removedJoystick);
                    al_release_joystick(removedJoystick);
                }
            }

            const Common::S32 joystickCount = al_get_num_joysticks();
            CONSOLE_INFOF("%u joysticks detected.", joystickCount);

            for (Common::S32 joystickID = 0; joystickID < joystickCount; ++joystickID)
            {
                ALLEGRO_JOYSTICK* joystick = al_get_joystick(joystickID);

                // Detect any added joysticks
                if (mAttachedJoysticks.find(joystick) == mAttachedJoysticks.end())
                {
                    mAttachedJoysticks.insert(mAttachedJoysticks.end(), joystick);
                    CONSOLE_INFO("New joystick attached");
                }

                const Common::S32 stickCount = al_get_joystick_num_sticks(joystick);
                const Common::S32 buttonCount = al_get_joystick_num_buttons(joystick);

                // Assign this joystick the ID
                mJoystickIDs[joystick] = joystickID;

                CONSOLE_DEBUGF("Joystick %u ------------", joystickID);
                CONSOLE_DEBUGF("  Name: %s", al_get_joystick_name(joystick));
                CONSOLE_DEBUGF("  Stick Count: %u", stickCount);
                CONSOLE_DEBUGF("  Button Count: %u", buttonCount);

                for (Common::U32 buttonID = 0; buttonID < buttonCount; ++buttonID)
                {
                    CONSOLE_DEBUGF("  Button %u -----------", buttonID);
                    CONSOLE_DEBUGF("    Name: %s", al_get_joystick_button_name(joystick, buttonID));
                }

                for (Common::U32 stickID = 0; stickID < stickCount; ++stickID)
                {
                    CONSOLE_DEBUGF("  Stick %u -----------", stickID);
                    CONSOLE_DEBUGF("    Name: %s", al_get_joystick_stick_name(joystick, stickID));
                }
            }
        }

        void SInputListener::setMouseCaptureEnabled(const bool enabled)
        {
            Engine::Video::SRenderer* renderer = Engine::Video::SRenderer::instantiate();

            if (renderer->mHasDisplay)
            {
                const bool result = enabled ? al_grab_mouse(renderer->getDisplay()) : al_ungrab_mouse();

                if (!result)
                    CONSOLE_ERROR("Failed to change mouse capture status.");
            }
        }

        void SInputListener::update(void)
        {
            PROFILER_BEGIN(Input);
            ALLEGRO_MOUSE_STATE mouseState;
            al_get_mouse_state(&mouseState);
            ALLEGRO_KEYBOARD_STATE keyboardState;
            al_get_keyboard_state(&keyboardState);

            CEGUI::GUIContext& guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

            // TODO: This is bound to go weird in some situations probably; like breaking responders for non-typable keys (like F1-F12)
            mTyping = guiContext.getInputCaptureWindow() != nullptr;

            Engine::GUI::SGUIManager* gui = Engine::GUI::SGUIManager::instantiate();
            gui->getContext("main")->setCursorPosition(Support::Vector2DF(mouseState.x, mouseState.y));

            // Process input events
            while (!al_is_event_queue_empty(mInputQueue))
            {
                ALLEGRO_EVENT event;

                if (al_get_next_event(mInputQueue, &event))
                {
                    switch (event.type)
                    {
                        default:
                            break;

                        // Keyboard key events
                        case ALLEGRO_EVENT_KEY_UP:
                        case ALLEGRO_EVENT_KEY_DOWN:
                        {
                            if (!mTyping)
                                this->dispatchInputResponse(event.keyboard.keycode, INPUT_KEYBOARD, 0, event);

                            // We'll just shove keyboard input into CEGUI, then.
                            if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN)
                                guiContext.injectKeyDown(AllegroKeyToCEGUI(event.keyboard.keycode));
                            else
                                guiContext.injectKeyUp(AllegroKeyToCEGUI(event.keyboard.keycode));

                            break;
                        }

                        // Mouse Events
                        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                        {
                            this->dispatchInputResponse(event.mouse.button, INPUT_MOUSE, 0, event);

                            if (event.mouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                                guiContext.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
                            else
                                guiContext.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);

                            break;
                        }

                        // Joystick events
                        case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
                        {
                            CONSOLE_INFO("Detected change in joystick configuration.");

                            this->scanJoysticks();
                            break;
                        }

                        case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                        case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                        {
                            auto joystickIDSearch = mJoystickIDs.find(event.joystick.id);
                            assert(joystickIDSearch != mJoystickIDs.end());

                            this->dispatchInputResponse(event.joystick.button, INPUT_GAMEPAD, (*joystickIDSearch).second, event);
                            break;
                        }

                        case ALLEGRO_EVENT_JOYSTICK_AXIS:
                        {
                            auto joystickIDSearch = mJoystickIDs.find(event.joystick.id);
                            assert(joystickIDSearch != mJoystickIDs.end());

                            const Common::U32 inputID = event.joystick.stick + al_get_joystick_num_buttons(event.joystick.id);
                            this->dispatchInputResponse(inputID, INPUT_GAMEPAD, (*joystickIDSearch).second, event);
                            break;
                        }
                    }
                }
            }

            PROFILER_END(Input);
        }

        bool SInputListener::setInputScheme(const Support::String& name)
        {
            auto search = mInputSchemes.find(name);

            if (search == mInputSchemes.end())
                return false;

            mInputScheme = name;
            return true;
        }

        const Support::String& SInputListener::getInputScheme(void)
        {
            return mInputScheme;
        }

        void SInputListener::setResponder(const Support::String& scheme, const Common::U32 inputCode, const INPUT_DEVICE deviceType, const Common::U32 deviceID, InputEventResponderDelegate* responder)
        {
            // We do a check on mouse and keyboard input types, the deviceID should be 0 because we don't individually identify these devices.
            switch(deviceType)
            {
                case INPUT_MOUSE:
                case INPUT_KEYBOARD:
                {
                    if (deviceID != 0)
                        throw std::runtime_error("When binding mouse and keybord devices, the device ID must be 0!");
                    break;
                }
            }

            // Initialize the scheme if not found
            auto schemeSearch = mInputSchemes.find(scheme);
            if (schemeSearch == mInputSchemes.end())
                mInputSchemes[scheme] =  Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, Support::UnorderedMap<Common::U32, InputEventResponderDelegate*>>>();

            auto& schemeData = mInputSchemes[scheme];

            // Initialize the input device data if not found
            auto deviceTypeSearch = schemeData.find(deviceType);
            if (deviceTypeSearch == schemeData.end())
                schemeData[deviceType] = Support::UnorderedMap<Common::U32, Support::UnorderedMap<Common::U32, InputEventResponderDelegate*>>();

            auto& inputDeviceTypeData = schemeData[deviceType];

            // Initialize the device ID data if not found
            auto deviceSearch = inputDeviceTypeData.find(deviceID);
            if (deviceSearch == inputDeviceTypeData.end())
                inputDeviceTypeData[deviceID] = Support::UnorderedMap<Common::U32, InputEventResponderDelegate*>();

            auto& responderData = inputDeviceTypeData[deviceID];

            // Finally insert the freaking lookup
            responderData[inputCode] = responder;
        }

        void SInputListener::dispatchInputResponse(const Common::U32 inputCode, const INPUT_DEVICE deviceType, const Common::U32 deviceID, ALLEGRO_EVENT& event)
        {
            auto schemeSearch = mInputSchemes.find(mInputScheme);
            if (schemeSearch == mInputSchemes.end())
                return;

            auto& schemeData = (*schemeSearch).second;
            auto deviceTypeSearch = schemeData.find(deviceType);
            if (deviceTypeSearch == schemeData.end())
                return;

            auto& deviceTypeData = (*deviceTypeSearch).second;
            auto deviceIDSearch = deviceTypeData.find(deviceID);
            if (deviceIDSearch == deviceTypeData.end())
                return;

            auto& deviceIDData = (*deviceIDSearch).second;
            auto inputCodeSearch = deviceIDData.find(inputCode);
            if (inputCodeSearch == deviceIDData.end())
                return;

            (*inputCodeSearch).second->invoke(event);
        }
    } // End Namespace Game
} // End Namespace Kiaro
