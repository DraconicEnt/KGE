/*
 *
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