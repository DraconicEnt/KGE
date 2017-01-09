/**
 *  @file CDisplay.hpp
 *  @brief Include file declaring the CDisplay class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_VIDEO_CGRAPHICSWINDOW_HPP_
#define _INCLUDE_VIDEO_CGRAPHICSWINDOW_HPP_

#include <allegro5/allegro.h>
#include <support/types.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            /**
             *  @brief A class representing a graphics window that can be drawn to. These are used with the SRenderer class.
             */
            class CGraphicsWindow
            {
                public:
                    //! A struct representing window initialization parameters.
                    struct WindowParameters
                    {
                        //! The title of the window.
                        Support::String mTitle;

                        //! Whether or not the window should be fullscreen.
                        bool mFullscreen;

                        //! The resolution of the new window.
                        Support::Dimension2DU mResolution;
                    };

                private:
                    //! The allegro display in use with this window.
                    ALLEGRO_DISPLAY* mDisplay;

                    //! A pointer to the event queue for our Allegro display.
                    ALLEGRO_EVENT_QUEUE* mWindowEventQueue;

                public:
                    //! The initialization parameters this CGraphicsWindow was created with.
                    const WindowParameters mWindowParameters;

                public:
                    CGraphicsWindow(const WindowParameters& parameters);
                    CGraphicsWindow(WindowParameters* parameters);

                    //! Standard destructor.
                    ~CGraphicsWindow(void);

                    //! Initializes the CGraphicsWindow, displaying it.
                    bool initialize(void);

                    //! Sets the OpenGL context associated with this display as the active context.
                    void setActiveOpenGLContext(void);

                    //! Swaps the current backbuffer contents with the frontbuffer.
                    void swapBuffers(void);

                    /**
                     *  @brief Sets a new window title.
                     *  @param title The new window title to use.
                     */
                    void setTitle(const Support::String& title);

                    //! Process all pending events for the game window, such as window resizing.
                    void processWindowEvents(void);
            };
        }
    }
}
#endif // _INCLUDE_VIDEO_CDISPLAY_HPP_
