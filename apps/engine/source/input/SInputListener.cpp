/**
 *  @file SInputListener.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <game/MoveManager.hpp>
#include <input/SInputListener.hpp>

#include <support/Logging.hpp>

namespace Kiaro
{
    namespace Input
    {
        static SInputListener* sInstance = NULL;

        SInputListener* SInputListener::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SInputListener;

            return sInstance;
        }

        void SInputListener::destroy(void)
        {
            if (sInstance)
                delete sInstance;

            sInstance = NULL;
        }

        void SInputListener::setKeyResponder(const CEGUI::Key::Scan& key, KeyResponderPointer responder)
        {
            if (!responder)
            {
                mKeyResponders.erase(key);
                return;
            }

            mKeyResponders[key] = responder;
        }

        SInputListener::SInputListener(void)
        {
            // Ensure that the key states all exist
            for (Common::U8 iteration = 0; iteration <= 254; iteration++)
                mKeyStates[iteration] = false;

            mKeyStates[255] = false;
        }

        SInputListener::~SInputListener(void)
        {

        }

        static inline CEGUI::Key::Scan irrlichtKeyToCEGUI(const irr::EKEY_CODE& code)
        {
            switch(code)
            {
                case irr::EKEY_CODE::KEY_KEY_A:
                    return CEGUI::Key::Scan::A;
                case irr::EKEY_CODE::KEY_KEY_B:
                    return CEGUI::Key::Scan::B;
                case irr::EKEY_CODE::KEY_KEY_C:
                    return CEGUI::Key::Scan::C;
                case irr::EKEY_CODE::KEY_KEY_D:
                    return CEGUI::Key::Scan::D;
                case irr::EKEY_CODE::KEY_KEY_E:
                    return CEGUI::Key::Scan::E;
                case irr::EKEY_CODE::KEY_KEY_F:
                    return CEGUI::Key::Scan::F;
                case irr::EKEY_CODE::KEY_KEY_G:
                    return CEGUI::Key::Scan::G;
                case irr::EKEY_CODE::KEY_KEY_H:
                    return CEGUI::Key::Scan::H;
                case irr::EKEY_CODE::KEY_KEY_I:
                    return CEGUI::Key::Scan::I;
                case irr::EKEY_CODE::KEY_KEY_J:
                    return CEGUI::Key::Scan::J;
                case irr::EKEY_CODE::KEY_KEY_K:
                    return CEGUI::Key::Scan::K;
                case irr::EKEY_CODE::KEY_KEY_L:
                    return CEGUI::Key::Scan::L;
                case irr::EKEY_CODE::KEY_KEY_M:
                    return CEGUI::Key::Scan::M;
                case irr::EKEY_CODE::KEY_KEY_N:
                    return CEGUI::Key::Scan::N;
                case irr::EKEY_CODE::KEY_KEY_O:
                    return CEGUI::Key::Scan::O;
                case irr::EKEY_CODE::KEY_KEY_P:
                    return CEGUI::Key::Scan::P;
                case irr::EKEY_CODE::KEY_KEY_Q:
                    return CEGUI::Key::Scan::Q;
                case irr::EKEY_CODE::KEY_KEY_R:
                    return CEGUI::Key::Scan::R;
                case irr::EKEY_CODE::KEY_KEY_S:
                    return CEGUI::Key::Scan::S;
                case irr::EKEY_CODE::KEY_KEY_T:
                    return CEGUI::Key::Scan::T;
                case irr::EKEY_CODE::KEY_KEY_U:
                    return CEGUI::Key::Scan::U;
                case irr::EKEY_CODE::KEY_KEY_V:
                    return CEGUI::Key::Scan::V;
                case irr::EKEY_CODE::KEY_KEY_W:
                    return CEGUI::Key::Scan::W;
                case irr::EKEY_CODE::KEY_KEY_X:
                    return CEGUI::Key::Scan::X;
                case irr::EKEY_CODE::KEY_KEY_Y:
                    return CEGUI::Key::Scan::Y;
                case irr::EKEY_CODE::KEY_KEY_Z:
                    return CEGUI::Key::Scan::Z;

                case irr::EKEY_CODE::KEY_KEY_0:
                    return CEGUI::Key::Scan::Zero;
                case irr::EKEY_CODE::KEY_KEY_1:
                    return CEGUI::Key::Scan::One;
                case irr::EKEY_CODE::KEY_KEY_2:
                    return CEGUI::Key::Scan::Two;
                case irr::EKEY_CODE::KEY_KEY_3:
                    return CEGUI::Key::Scan::Three;
                case irr::EKEY_CODE::KEY_KEY_4:
                    return CEGUI::Key::Scan::Four;
                case irr::EKEY_CODE::KEY_KEY_5:
                    return CEGUI::Key::Scan::Five;
                case irr::EKEY_CODE::KEY_KEY_6:
                    return CEGUI::Key::Scan::Six;
                case irr::EKEY_CODE::KEY_KEY_7:
                    return CEGUI::Key::Scan::Seven;
                case irr::EKEY_CODE::KEY_KEY_8:
                    return CEGUI::Key::Scan::Eight;
                case irr::EKEY_CODE::KEY_KEY_9:
                    return CEGUI::Key::Scan::Nine;

                case irr::EKEY_CODE::KEY_MINUS:
                    return CEGUI::Key::Scan::Minus;
                case irr::EKEY_CODE::KEY_PLUS:
                    return CEGUI::Key::Scan::Add;

                case irr::EKEY_CODE::KEY_TAB:
                    return CEGUI::Key::Scan::Tab;

                case irr::EKEY_CODE::KEY_ESCAPE:
                    return CEGUI::Key::Scan::Escape;
                case irr::EKEY_CODE::KEY_F1:
                    return CEGUI::Key::Scan::F1;
                case irr::EKEY_CODE::KEY_F2:
                    return CEGUI::Key::Scan::F2;
                case irr::EKEY_CODE::KEY_F3:
                    return CEGUI::Key::Scan::F3;
                case irr::EKEY_CODE::KEY_F4:
                    return CEGUI::Key::Scan::F4;
                case irr::EKEY_CODE::KEY_F5:
                    return CEGUI::Key::Scan::F5;
                case irr::EKEY_CODE::KEY_F6:
                    return CEGUI::Key::Scan::F6;
                case irr::EKEY_CODE::KEY_F7:
                    return CEGUI::Key::Scan::F7;
                case irr::EKEY_CODE::KEY_F8:
                    return CEGUI::Key::Scan::F8;
                case irr::EKEY_CODE::KEY_F9:
                    return CEGUI::Key::Scan::F9;
                case irr::EKEY_CODE::KEY_F10:
                    return CEGUI::Key::Scan::F10;
                case irr::EKEY_CODE::KEY_F11:
                    return CEGUI::Key::Scan::F11;
                case irr::EKEY_CODE::KEY_F12:
                    return CEGUI::Key::Scan::F12;

                case irr::EKEY_CODE::KEY_UP:
                    return CEGUI::Key::Scan::ArrowUp;
                case irr::EKEY_CODE::KEY_DOWN:
                    return CEGUI::Key::Scan::ArrowDown;
                case irr::EKEY_CODE::KEY_LEFT:
                    return CEGUI::Key::Scan::ArrowLeft;
                case irr::EKEY_CODE::KEY_RIGHT:
                    return CEGUI::Key::Scan::ArrowRight;

                /*
                case irr::EKEY_CODE::KEY_:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                case irr::EKEY_CODE::KEY_SPACE:
                    return CEGUI::Key::Scan::Space;
                    */

            }

            return CEGUI::Key::Scan::Unknown;
        }

        bool SInputListener::OnEvent(const irr::SEvent& event)
        {
            switch(event.EventType)
            {
                // Keyboard input
                case irr::EET_KEY_INPUT_EVENT:
                {
                    const CEGUI::Key::Scan key = irrlichtKeyToCEGUI(event.KeyInput.Key);

                    CEGUI::GUIContext& guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

                    if (event.KeyInput.PressedDown)
                    {
                        guiContext.injectKeyDown(key);

                        if (mKeyResponders.count(key) && !mKeyStates[key])
                            mKeyResponders[key](true);

                        mKeyStates[key] = true;
                    }
                    else
                    {
                        guiContext.injectKeyUp(key);

                        if (mKeyResponders.count(key) && mKeyStates[key])
                            mKeyResponders[key](false);

                        mKeyStates[key] = false;
                    }

                    break;
                }

                // Mouse Input
                case irr::EET_MOUSE_INPUT_EVENT:
                {
                    CEGUI::GUIContext& guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

                    switch(event.MouseInput.Event)
                    {
                        case irr::EMIE_MOUSE_MOVED:
                        {
                            guiContext.injectMousePosition(event.MouseInput.X, event.MouseInput.Y);
                            return true;
                        }

                        case irr::EMIE_LMOUSE_PRESSED_DOWN:
                        {
                            guiContext.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
                            return true;
                        }

                        case irr::EMIE_LMOUSE_LEFT_UP:
                        {
                            guiContext.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
                            return true;
                        }
                    }

                    return false;
                }

                // Logging
                case irr::EET_LOG_TEXT_EVENT:
                {
                    Support::Logging::write(Support::Logging::MESSAGE_INFO, "SInputListener: %s", event.LogEvent.Text);
                    return true;
                }
            }

            return false;
        }
    } // End Namespace Game
} // End Namespace Kiaro
