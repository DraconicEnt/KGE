/**
 *  @file CDisplay.cpp
 *  @brief Source code file implementing the CDisplay class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <video/CDisplay.hpp>

#include <allegro5/allegro_opengl.h>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            CDisplay::CDisplay(const Support::String& title)
            {
                al_set_new_window_title(title.data());
                al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_OPENGL);

                // Initialize the display
                mDisplay = al_create_display(1024, 768);
            }

            void CDisplay::draw(void)
            {
                al_set_current_opengl_context(mDisplay);

            }
        }
    }
}
