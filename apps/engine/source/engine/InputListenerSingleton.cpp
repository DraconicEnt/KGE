/**
 *  @file InputListenerSingleton.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */


#include <CEGUI/CEGUI.h>
#include <engine/InputListenerSingleton.hpp>

namespace Kiaro
{
    namespace Engine
    {
        static InputListenerSingleton *InputListenerSingleton_Instance = NULL;

        InputListenerSingleton *InputListenerSingleton::getPointer(void)
        {
            if (!InputListenerSingleton_Instance)
                InputListenerSingleton_Instance = new InputListenerSingleton;

            return InputListenerSingleton_Instance;
        }

        void InputListenerSingleton::destroy(void)
        {
            if (InputListenerSingleton_Instance)
                delete InputListenerSingleton_Instance;

            InputListenerSingleton_Instance = NULL;
        }

        InputListenerSingleton::InputListenerSingleton(void) { }

        InputListenerSingleton::~InputListenerSingleton(void) { }

        bool InputListenerSingleton::OnEvent(const irr::SEvent& event)
        {
            if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
            {
                CEGUI::GUIContext &guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

                switch(event.MouseInput.Event)
                {
                    case irr::EMIE_MOUSE_MOVED:
                    {
                        guiContext.injectMousePosition(event.MouseInput.X, event.MouseInput.Y);
                        break;
                    }

                    case irr::EMIE_LMOUSE_PRESSED_DOWN:
                    {
                        guiContext.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
                        break;
                    }

                    case irr::EMIE_LMOUSE_LEFT_UP:
                    {
                        guiContext.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
                        break;
                    }
                }
            }
        }
    } // End Namespace Game
} // End Namespace Kiaro
