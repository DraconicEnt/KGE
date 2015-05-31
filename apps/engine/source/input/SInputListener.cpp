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


#include <CEGUI/CEGUI.h>

#include <input/SInputListener.hpp>

namespace Kiaro
{
    namespace Input
    {
        static SInputListener *InputListenerSingleton_Instance = NULL;

        SInputListener *SInputListener::getPointer(void)
        {
            if (!InputListenerSingleton_Instance)
                InputListenerSingleton_Instance = new SInputListener;

            return InputListenerSingleton_Instance;
        }

        void SInputListener::destroy(void)
        {
            if (InputListenerSingleton_Instance)
                delete InputListenerSingleton_Instance;

            InputListenerSingleton_Instance = NULL;
        }

        SInputListener::SInputListener(void) { }

        SInputListener::~SInputListener(void) { }

        bool SInputListener::OnEvent(const irr::SEvent& event)
        {
            switch(event.EventType)
            {
                // Mouse Input
                case irr::EET_MOUSE_INPUT_EVENT:
                {
                    CEGUI::GUIContext &guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

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
                    std::cout << "SInputListener: " << event.LogEvent.Text << std::endl;
                    return true;
                }
            }

            return false;
        }
    } // End Namespace Game
} // End Namespace Kiaro
