/**
 *  @file SRenderer.cpp
 *  @brief Source file containing the implementation for the Video::SRenderer singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <video/SRenderer.hpp>

#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

#if defined(ENGINE_UNIX)
#include <allegro5/allegro_x.h>
#elif defined(ENGINE_WIN)
#include <allegro5/allegro_windows.h>
#else
#include <allegro5/allegro_osx.h>
#endif

#include <support/SSynchronousScheduler.hpp>

#include <support/SSettingsRegistry.hpp>
#include <video/CSceneGraph.hpp>

#include <support/SProfiler.hpp>

#include <gui/SGUIManager.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            SRenderer::SRenderer(void) : mIrrlichtDevice(nullptr), mClearColor(Common::ColorRGBA(0, 0, 0, 0)),
                                         mHasDisplay(!Core::SEngineInstance::getPointer()->isDedicated()),
                                         mVideo(nullptr), mSceneManager(nullptr), mMainScene(nullptr),
                                         mCurrentScene(nullptr), mDisplay(nullptr), mWindowEventQueue(nullptr),
                                         mTimePulse(nullptr)
            {
                CONSOLE_INFO("Initializing renderer subsystem.");

                Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getPointer();
                irr::core::dimension2d<Common::U32> resolution = settings->getValue<irr::core::dimension2d<Common::U32>>(
                        "Video::Resolution");
                this->initializeRenderer(resolution);
            }

            SRenderer::~SRenderer(void)
            {
                if (mIrrlichtDevice)
                {
                    mIrrlichtDevice->drop();
                    mIrrlichtDevice = nullptr;
                }

                if (mDisplay)
                {
                    al_destroy_display(mDisplay);
                    mDisplay = nullptr;
                }
            }

            Common::S32 SRenderer::initializeRenderer(const Support::Dimension2DU &resolution)
            {
                irr::video::E_DRIVER_TYPE videoDriver = irr::video::EDT_OPENGL;
                irr::SIrrlichtCreationParameters creationParameters;

                Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getPointer();

                if (mHasDisplay)
                {
                    const Common::S32 monitorCount = al_get_num_video_adapters();
                    CONSOLE_INFOF("Detected %u monitor(s)", monitorCount);

                    // Create the Allegro window and get its ID
                    Common::S32 displayFlags = ALLEGRO_GENERATE_EXPOSE_EVENTS | ALLEGRO_RESIZABLE;
                    if (settings->getValue<bool>("Video::Fullscreen"))
                        displayFlags |= ALLEGRO_FULLSCREEN;

                    al_set_new_display_flags(displayFlags);

                    CONSOLE_INFOF("Using %ux%u resolution.", resolution.Width, resolution.Height);
                    mDisplay = al_create_display(resolution.Width, resolution.Height);

                    // TODO (Robert MacGregor#9): Use a preference for the desired screen
                    if (!mDisplay)
                    {
                        CONSOLE_ERROR("Failed to create Allegro display!");
                        return 1;
                    }

                    mWindowEventQueue = al_create_event_queue();
                    al_register_event_source(mWindowEventQueue, al_get_display_event_source(mDisplay));

                    // TODO (Robert MacGregor#9): Rename to game name?
                    al_set_window_title(mDisplay, "Kiaro Game Engine");
                    al_set_new_window_title("Auxiliary Display ");
                    al_hide_mouse_cursor(mDisplay);

    #if defined(ENGINE_UNIX)
                    XID windowID = al_get_x_window_id(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void *>(windowID);
    #elif defined(ENGINE_WIN)
                    HWND windowHandle = al_get_win_window_handle(mDisplay);
                                    creationParameters.WindowId = reinterpret_cast<void*>(windowHandle);
    #endif
                }
                else
                    videoDriver = irr::video::EDT_NULL;

                // Setup the Irrlicht creation request
                creationParameters.Bits = 32;
                creationParameters.IgnoreInput = true; // We will use Allegro for this
                creationParameters.DriverType = videoDriver;
                creationParameters.Doublebuffer = true;

    #if defined(ENGINE_WIN)
                // The standard Win32 windowing should work here
                creationParameters.DeviceType = irr::EIDT_WIN32;
    #else
                // We should be using SDL on Linux with this as the GLX routines used in the X implementation are not supported by NVidia drivers
                creationParameters.DeviceType = irr::EIDT_SDL;
    #endif

                creationParameters.WindowSize = resolution;

                mIrrlichtDevice = irr::createDeviceEx(creationParameters);

                if (!mIrrlichtDevice)
                {
                    CONSOLE_ERROR("Failed to initialize Irrlicht! (Does your Irrlicht lib support SDL?)");
                    return 2;
                }

                // Grab the scene manager and store it to reduce a function call
                mSceneManager = mIrrlichtDevice->getSceneManager();
                mVideo = mIrrlichtDevice->getVideoDriver();
                mSceneManager->setActiveCamera(mSceneManager->addCameraSceneNode());

                // Setup the main scene and make it the active scene
                mMainScene = mCurrentScene = new CSceneGraph(this);

                // Set up the renderer time pulse
                if (mHasDisplay)
                {
                    const Common::U16 activeFPS = settings->getValue<Common::U16>("Video::ActiveFPS");

                    mTimePulse = Support::SSynchronousScheduler::getPointer()->schedule(Support::FPSToMS(activeFPS), true,
                                                                                        this,
                                                                                        &SRenderer::drawFrame);
                }

                CONSOLE_INFOF("Irrlicht version is %s.", mIrrlichtDevice->getVersion());
                CONSOLE_INFO("Initialized renderer.");
                return 0;
            }

            void SRenderer::setSceneGraph(CSceneGraph *graph)
            {
                if (mCurrentScene)
                    mCurrentScene->setVisible(false);

                mCurrentScene = graph;

                if (mCurrentScene)
                    mCurrentScene->setVisible(true);
            }

            void SRenderer::setResolution(const Support::Dimension2DU &resolution)
            {
                al_resize_display(mDisplay, resolution.Width, resolution.Height);
                mIrrlichtDevice->getVideoDriver()->OnResize(resolution);
                GUI::SGUIManager::getPointer()->setResolution(resolution);
                al_acknowledge_resize(mDisplay);
            }

            irr::IrrlichtDevice *SRenderer::getIrrlichtDevice(void) const
            {
                return mIrrlichtDevice;
            }

            CSceneGraph *SRenderer::getMainScene(void)
            {
                return mMainScene;
            }

            CSceneGraph *SRenderer::getCurrentScene(void)
            {
                return mCurrentScene;
            }

            void SRenderer::processWindowEvents(void)
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
                            Core::SEngineInstance::getPointer()->kill();
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_RESIZE:
                        {
                            if (!al_acknowledge_resize(mDisplay))
                                CONSOLE_WARNING("Failed to resize display!");
                            else
                            {
                                // What is the new display dimensions?
                                Common::S32 displayWidth = al_get_display_width(mDisplay);
                                Common::S32 displayHeight = al_get_display_height(mDisplay);
                                this->setResolution(Support::Dimension2DU(displayWidth, displayHeight));
                            }

                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                        {
                            CONSOLE_INFO("Window unfocused.");
                            Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getPointer();
                            const Common::U16 inactiveFPS = settings->getValue<Common::U16>("Video::InactiveFPS");

                            mTimePulse->setWaitTimeMS(Support::FPSToMS(inactiveFPS), true);
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                        {
                            CONSOLE_INFO("Window focused.");
                            Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getPointer();
                            const Common::U16 activeFPS = settings->getValue<Common::U16>("Video::ActiveFPS");

                            mTimePulse->setWaitTimeMS(Support::FPSToMS(activeFPS), true);
                            break;
                        }
                    }
            }

            ALLEGRO_DISPLAY *SRenderer::getDisplay(void)
            {
                return mDisplay;
            }

            void SRenderer::drawFrame(void)
            {
                PROFILER_BEGIN(Render);
                mVideo->beginScene(true, true, mClearColor);

                if (mCurrentScene)
                    mSceneManager->drawAll();

                GUI::SGUIManager::getPointer()->draw();

                mVideo->endScene();

                // FIXME: On Windows, we don't want to call al_flip_display because of framerate issues. We should figure out why that is.
    #if !defined(ENGINE_WIN)
                al_flip_display();
    #endif

                this->processWindowEvents();
                al_inhibit_screensaver(true);

                PROFILER_END(Render);
            }
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
