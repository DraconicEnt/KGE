/**
 *  @file CGraphicsWindow.cpp
 *  @brief Source code file implementing the CGraphicsWindow class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <video/CGraphicsWindow.hpp>

#include <support/Console.hpp>
#include <core/SEngineInstance.hpp>
#include <support/SSettingsRegistry.hpp>
#include <allegro5/allegro_opengl.h>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            CGraphicsWindow::CGraphicsWindow(const WindowParameters& parameters) : mWindowParameters(parameters) { }

            CGraphicsWindow::CGraphicsWindow(WindowParameters* parameters) : mWindowParameters(*parameters) { }

            CGraphicsWindow::~CGraphicsWindow(void)
            {
                CONSOLE_ASSERT(mDisplay, "mDisplay should be non-NULL.");

                al_destroy_display(mDisplay);
                mDisplay = nullptr;
            }

            bool CGraphicsWindow::initialize(void)
            {
                CONSOLE_INFOF("Using %ux%u resolution.", mWindowParameters.mResolution.x(), mWindowParameters.mResolution.y());

                al_set_new_window_title(mWindowParameters.mTitle.data());

                Common::S32 displayFlags = ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 | ALLEGRO_GENERATE_EXPOSE_EVENTS;

                if (mWindowParameters.mFullscreen)
                    displayFlags |= ALLEGRO_FULLSCREEN;
                else
                    displayFlags |= ALLEGRO_WINDOWED;

                al_set_new_display_flags(displayFlags);
                mDisplay = al_create_display(mWindowParameters.mResolution.x(), mWindowParameters.mResolution.y());

                CONSOLE_ASSERT(mDisplay, "Failed to initialize Allegro display.");

                mWindowEventQueue = al_create_event_queue();
                CONSOLE_ASSERT(mWindowEventQueue, "Failed to initialize window event queue.");
                al_register_event_source(mWindowEventQueue, al_get_display_event_source(mDisplay));
                return true;
            }

            void CGraphicsWindow::setActiveOpenGLContext(void)
            {
                CONSOLE_ASSERT(mDisplay, "mDisplay should be non-NULL.");

                al_set_current_opengl_context(mDisplay);
            }

            void CGraphicsWindow::swapBuffers(void)
            {
                CONSOLE_ASSERT(mDisplay, "mDisplay should be non-NULL.");

                al_set_current_opengl_context(mDisplay);
                al_flip_display();
            }

            void CGraphicsWindow::setTitle(const Support::String& title)
            {
                CONSOLE_ASSERT(mDisplay, "mDisplay should be non-NULL.");

                al_set_window_title(mDisplay, title.data());
            }

            void CGraphicsWindow::processWindowEvents(void)
            {
                // Query for display events
                ALLEGRO_EVENT windowEvent;

                if (al_get_next_event(mWindowEventQueue, &windowEvent))
                    switch (windowEvent.type)
                    {
                        default:
                            break;

                        case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        {
                            Core::SEngineInstance::getInstance()->kill();
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_RESIZE:
                        {
                            if (!al_acknowledge_resize(mDisplay))
                                CONSOLE_WARNING("Failed to resize display!");
                            else
                            {
                                // FIXME: Properly process the resize events
                                // What is the new display dimensions?
                                Common::S32 displayWidth = al_get_display_width(mDisplay);
                                Common::S32 displayHeight = al_get_display_height(mDisplay);
                              //  this->setResolution(Support::Dimension2DU(displayWidth, displayHeight));
                            }

                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                        {
                            CONSOLE_INFO("Window unfocused.");
                            Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getInstance();
                            const Common::U16 inactiveFPS = settings->getValue<Common::U16>("Video::InactiveFPS");

                            // FIXME: Properly process the window event
                          //  mTimePulse->setWaitTimeMS(Support::FPSToMS(inactiveFPS), true);
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                        {
                            CONSOLE_INFO("Window focused.");
                            Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getInstance();
                            const Common::U16 activeFPS = settings->getValue<Common::U16>("Video::ActiveFPS");

                          //  mTimePulse->setWaitTimeMS(Support::FPSToMS(activeFPS), true);
                            break;
                        }
                    }
            }
        }
    }
}
