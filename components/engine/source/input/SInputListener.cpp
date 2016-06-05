/**
 *  @file SInputListener.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <allegro5/allegro.h>

#include <input/SInputListener.hpp>

#include <video/SRenderer.hpp>
#include <support/Console.hpp>

#include <support/SProfiler.hpp>

namespace Kiaro
{
    namespace Input
    {
        void SInputListener::setKeyResponder(const CEGUI::Key::Scan key, KeyResponderDelegate* responder)
        {
            if (!responder)
            {
                mKeyResponders.erase(key);
                return;
            }

            mKeyResponders[key] = responder;
        }

        SInputListener::SInputListener(void) : mTyping(false), mInputQueue(al_create_event_queue())
        {
            al_install_mouse();
            al_install_keyboard();
            al_install_joystick();
            al_register_event_source(mInputQueue, al_get_keyboard_event_source());
            al_register_event_source(mInputQueue, al_get_joystick_event_source());
            CONSOLE_INFO("Initialized input subsystem.");
            this->scanJoysticks();
        }

        SInputListener::~SInputListener(void)
        {
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_uninstall_joystick();
            al_destroy_event_queue(mInputQueue);
        }

        static inline CEGUI::Key::Scan AllegroKeyToCEGUI(const Common::U32 code)
        {
            switch(code)
            {
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
            al_reconfigure_joysticks();
            const Common::U32 joystickCount = al_get_num_joysticks();
            CONSOLE_INFOF("%u joysticks detected.", joystickCount);

            for (Common::U32 joystickID = 0; joystickID < joystickCount; joystickID++)
            {
                ALLEGRO_JOYSTICK* joystick = al_get_joystick(joystickID);
                const Common::U32 stickCount = al_get_joystick_num_sticks(joystick);
                const Common::U32 buttonCount = al_get_joystick_num_buttons(joystick);
                CONSOLE_DEBUGF("Joystick %u ------------", joystickID);
                CONSOLE_DEBUGF("  Name: %s", al_get_joystick_name(joystick));
                CONSOLE_DEBUGF("  Stick Count: %u", stickCount);
                CONSOLE_DEBUGF("  Button Count: %u", buttonCount);

                for (Common::U32 buttonID = 0; buttonID < buttonCount; buttonID++)
                {
                    CONSOLE_DEBUGF("  Button %u -----------", buttonID);
                    CONSOLE_DEBUGF("    Name: %s", al_get_joystick_button_name(joystick, buttonID));
                }

                for (Common::U32 stickID = 0; stickID < stickCount; stickID++)
                {
                    CONSOLE_DEBUGF("  Stick %u -----------", stickID);
                    CONSOLE_DEBUGF("    Name: %s", al_get_joystick_stick_name(joystick, stickID));
                }
            }
        }

        void SInputListener::setMouseCaptureEnabled(const bool enabled)
        {
            Video::SRenderer* renderer = Video::SRenderer::getPointer();

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
            guiContext.injectMousePosition(mouseState.x, mouseState.y);

            // Process keyboard events
            while (!al_is_event_queue_empty(mInputQueue))
            {
                ALLEGRO_EVENT event;

                if (al_get_next_event(mInputQueue, &event))
                {
                    switch (event.type)
                    {
                        case ALLEGRO_EVENT_KEY_UP:
                        case ALLEGRO_EVENT_KEY_DOWN:
                        {
                            // Responding to regular key events?
                            if (!mTyping && event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN)
                            {
                                auto it = mKeyResponders.find(AllegroKeyToCEGUI(event.keyboard.keycode));

                                if (it != mKeyResponders.end())
                                    if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN)
                                        (*it).second->invoke(true);
                                    else
                                        (*it).second->invoke(false);

                                break;
                            }

                            // We'll just shove keyboard input into CEGUI, then.
                            if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN)
                                guiContext.injectKeyDown(AllegroKeyToCEGUI(event.keyboard.keycode));
                            else
                                guiContext.injectKeyUp(AllegroKeyToCEGUI(event.keyboard.keycode));

                            break;
                        }

                        case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
                        {
                            CONSOLE_INFO("Detected change in joystick configuration.");
                            this->scanJoysticks();
                            break;
                        }
                    }
                }
            }

            PROFILER_END(Input);
        }
    } // End Namespace Game
} // End Namespace Kiaro
