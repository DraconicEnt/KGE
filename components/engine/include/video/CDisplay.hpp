//
// Created by ragora on 6/18/16.
//

#ifndef _INCLUDE_VIDEO_CDISPLAY_HPP_
#define _INCLUDE_VIDEO_CDISPLAY_HPP_

#include <allegro5/allegro.h>
#include <support/types.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            class CDisplay
            {
                public:
                    CDisplay(const Support::String& title);

                    void draw(void);

                    void setActiveDisplay(void);

                    ALLEGRO_DISPLAY* mDisplay;
            };
        }
    }
}
#endif // _INCLUDE_VIDEO_CDISPLAY_HPP_
