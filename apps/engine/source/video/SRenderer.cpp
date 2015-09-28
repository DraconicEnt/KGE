/**
 *  @file SRenderer.cpp
 */

#include <video/SRenderer.hpp>

#include <filesystem/SResourceProvider.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

#include <allegro5/allegro.h>

#if defined(ENGINE_UNIX)
    #include <allegro5/allegro_x.h>
#elif defined(ENGINE_WIN)
    #include <allegro5/allegro_windows.h>
#else
    #include <allegro5/allegro_osx.h>
#endif

#include <support/SSynchronousScheduler.hpp>

#include <support/SSettingsRegistry.hpp>
#include <input/SInputListener.hpp>
#include <video/CSceneGraph.hpp>
#include <support/Console.hpp>

#include <core/SEngineInstance.hpp>

namespace Kiaro
{
    namespace Video
    {
        static SRenderer* sInstance = NULL;

        SRenderer* SRenderer::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SRenderer;

            return sInstance;
        }

        void SRenderer::destroy(void)
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = NULL;
            }
        }

        int SRenderer::initializeRenderer(void)
        {
            irr::video::E_DRIVER_TYPE videoDriver = irr::video::EDT_OPENGL;
            Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

            irr::SIrrlichtCreationParameters creationParameters;

          //  if (mEngineMode != Core::MODE_DEDICATED)
          //  {
                Common::S32 monitorCount = al_get_num_video_adapters();
                Support::Console::write(Support::Console::MESSAGE_INFO, "SRenderer: Detected %u monitor(s)", monitorCount);

                // Create the Allegro window and get its ID
                al_set_new_display_flags(ALLEGRO_GENERATE_EXPOSE_EVENTS | ALLEGRO_RESIZABLE);

                irr::core::dimension2d<Common::U32> resolution = settings->getValue<irr::core::dimension2d<Common::U32>>("Video::Resolution");
                mDisplay = al_create_display(resolution.Width, resolution.Height);

                // TODO (Robert MacGregor#9): Use a preference for the desired screen
                if (!mDisplay)
                {
                    Support::Console::write(Support::Console::MESSAGE_FATAL, "SRenderer: Failed to create Allegro display!");
                    return 1;
                }

                mWindowEventQueue = al_create_event_queue();
                al_register_event_source(mWindowEventQueue, al_get_display_event_source(mDisplay));

                // TODO (Robert MacGregor#9): Rename to game name?
                al_set_window_title(mDisplay, "Kiaro Game Engine");
                al_set_new_window_title("Auxiliary Display ");

                #if defined(ENGINE_UNIX)
                    XID windowID = al_get_x_window_id(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void*>(windowID);
                #elif defined(ENGINE_WIN)
                    HWND windowHandle = al_get_win_window_handle(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void*>(windowHandle);
                #else
                    NSWindow* windowHandle = al_osx_get_window(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void*>(windowHandle);
                #endif
          //  }
          //  else
           //     videoDriver = irr::video::EDT_NULL;

            // Setup the Irrlicht creation request
            creationParameters.Bits = 32;
            creationParameters.IgnoreInput = false; // We will use Allegro for this
            creationParameters.DriverType = videoDriver;
            creationParameters.DeviceType = irr::EIDT_SDL;
            creationParameters.WindowSize = resolution;

            mIrrlichtDevice = irr::createDeviceEx(creationParameters);

            if (!mIrrlichtDevice)
            {
                Support::Console::write(Support::Console::MESSAGE_FATAL, "SRenderer: Failed to initialize Irrlicht! (Does your Irrlicht lib support SDL?)");
                return 2;
            }

            // Grab the scene manager and store it to reduce a function call
            mSceneManager = mIrrlichtDevice->getSceneManager();
            mVideo = mIrrlichtDevice->getVideoDriver();
            mSceneManager->setActiveCamera(mSceneManager->addCameraSceneNode());

            // Initialize the main scene and set it
            // TODO (Robert MacGregor#9): Only initialize when running as a client.
           // mMainScene = new Video::CSceneGraph();
           // this->setSceneGraph(mMainScene);

            // Set up the renderer time pulse
            mTimePulse = Support::SSynchronousScheduler::getPointer()->schedule(16, true, this, &SRenderer::drawFrame);

            Support::Console::write(Support::Console::MESSAGE_INFO, "SRenderer: Irrlicht version is %s.", mIrrlichtDevice->getVersion());
            Support::Console::write(Support::Console::MESSAGE_INFO, "SRenderer: Initialized renderer.");

            return 0;
        }

        SRenderer::SRenderer(void) : mClearColor(Common::ColorRGBA(0, 0, 0, 0))
        {
            this->initializeRenderer();
            this->initializeGUI();
        }

        void SRenderer::setSceneGraph(CSceneGraph* graph)
        {
            if (mCurrentScene)
                mCurrentScene->setVisible(false);

            mCurrentScene = graph;
            mCurrentScene->setVisible(true);
        }

        SRenderer::~SRenderer(void)
        {
            if (mIrrlichtDevice)
            {
                mIrrlichtDevice->drop();
                mIrrlichtDevice = NULL;
            }

            if (mDisplay)
            {
                al_destroy_display(mDisplay);
                mDisplay = NULL;
            }
        }

        void SRenderer::setResolution(const Support::Dimension2DU& resolution)
        {
            al_resize_display(mDisplay, resolution.Width, resolution.Height);

            mIrrlichtDevice->getVideoDriver()->OnResize(resolution);
            CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(resolution.Width, resolution.Height));

            al_acknowledge_resize(mDisplay);
        }

        int SRenderer::initializeGUI(void)
        {
            // Start up CEGUI (if we're a client)
          //  if (mEngineMode == MODE_CLIENTCONNECT || mEngineMode == MODE_CLIENT)
           // {
                try
                {
                    // We don't need the OS cursor
                    mIrrlichtDevice->getCursorControl()->setVisible(false);

                    CEGUI::IrrlichtRenderer& renderer = CEGUI::IrrlichtRenderer::create(*mIrrlichtDevice);
                    FileSystem::SResourceProvider *resourceProvider = FileSystem::SResourceProvider::getPointer();


                    CEGUI::System::create(renderer, resourceProvider, NULL, NULL, NULL, "", "log.txt");

                    resourceProvider->setResourceGroupDirectory("fonts", "fonts/");
                    resourceProvider->setResourceGroupDirectory("ui", "ui/");

                    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "ui");
                    CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font", "fonts" );

                    // Set the defaults
                    CEGUI::GUIContext& guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

                    guiContext.setDefaultFont( "DejaVuSans-10" );
                    guiContext.getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );
                    guiContext.getMouseCursor().setImage(guiContext.getMouseCursor().getDefaultImage());

                    Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Initialized the GUI system.");
                }
                catch (CEGUI::InvalidRequestException& e)
                {
                    Support::Console::write(Support::Console::MESSAGE_FATAL, "SRenderer: Failed to initialize the GUI System. Reason:\n%s", e.what());
                    return 1;
                }
          //  }

            return 0;
        }

        void SRenderer::processWindowEvents(void)
        {
            // Query for display events
            ALLEGRO_EVENT windowEvent;

            if (al_get_next_event(mWindowEventQueue, &windowEvent))
                switch (windowEvent.type)
                    {
                        case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        {
                            Core::SEngineInstance::getPointer()->kill();
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_RESIZE:
                        {
                            if (!al_acknowledge_resize(mDisplay))
                                Support::Console::write(Support::Console::MESSAGE_WARNING, "SRenderer: Failed to resize display!");
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
                            Support::Console::write(Support::Console::MESSAGE_INFO, "SRenderer: Window unfocused.");

                            // Adjust our framerate to something lower if the window isn't focused
                            mTimePulse->setWaitTimeMS(64, true);

                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                        {
                            Support::Console::write(Support::Console::MESSAGE_INFO, "SRenderer: Window focused.");

                            mTimePulse->setWaitTimeMS(16, true);

                            break;
                        }
                    }
        }

        void SRenderer::drawFrame(void)
        {
            // What is our current display size?
          //  irr::core::dimension2d<irr::u32> lastDisplaySize = mIrrlichtDevice->getVideoDriver()->getScreenSize();

                      //  irr::core::dimension2d<Common::U32> currentDisplaySize = mIrrlichtDevice->getVideoDriver()->getScreenSize();

                        // Be sure to notify all the subsystems of our window resizing
                       // if (lastDisplaySize != currentDisplaySize)
                       // {
                         //   CEGUI::Sizef newDisplaySize(currentDisplaySize.Width, currentDisplaySize.Height);
                         //   CEGUI::System::getSingleton().notifyDisplaySizeChanged(newDisplaySize);

                         //   lastDisplaySize = currentDisplaySize;
                      //  }

                             //                 if (mDisplay)
                          // this->processWindowEvents();

            mVideo->beginScene(true, true, mClearColor);

            mSceneManager->drawAll();
            CEGUI::System::getSingleton().renderAllGUIContexts();

            mVideo->endScene();

            al_flip_display();

            this->processWindowEvents();
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
