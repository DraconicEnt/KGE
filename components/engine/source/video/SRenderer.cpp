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


#include <support/SSynchronousScheduler.hpp>

#include <support/SSettingsRegistry.hpp>
#include <video/CSceneGraph.hpp>
#include <video/CGraphicsWindow.hpp>
#include <video/COSGWindow.hpp>
#include <support/SProfiler.hpp>

#include <gui/SGUIManager.hpp>

#include <allegro5/allegro_opengl.h>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            SRenderer::SRenderer(void) : mViewer(nullptr), mClearColor(Support::ColorRGBA(0, 0, 0, 0)),
                                         mHasDisplay(!Core::SEngineInstance::getInstance()->isDedicated()),
                                         mMainScene(nullptr),mCurrentScene(nullptr), mPrimaryWindow(nullptr), mTimePulse(nullptr)
            {
                CONSOLE_INFO("Initializing renderer subsystem.");

                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getInstance();
                Support::Dimension2DU resolution = settings->getValue<Support::Dimension2DU>(
                        "Video::Resolution");
                this->initializeRenderer(resolution);
            }

            SRenderer::~SRenderer(void)
            {
                if (mViewer)
                {
                    delete mViewer;
                    mViewer = nullptr;
                }

                if (mPrimaryWindow)
                {
                    delete mPrimaryWindow;
                    mPrimaryWindow = nullptr;
                }
            }

            Common::S32 SRenderer::initializeRenderer(const Support::Dimension2DU& resolution)
            {
                Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getInstance();

                if (mHasDisplay)
                {
                    Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

                    CGraphicsWindow::WindowParameters parameters;
                    parameters.mTitle = "Kiaro Game Engine";
                    parameters.mFullscreen = settings->getValue<bool>("Video::Fullscreen");
                    parameters.mResolution = settings->getValue<Support::Dimension2DU>("Video::Resolution");

                    mPrimaryWindow = new CGraphicsWindow(parameters);
                    mPrimaryWindow->initialize();

                    mViewer = new osgViewer::Viewer();
                    mViewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::DrawThreadPerContext);

                    COSGWindow* osgWindow = new COSGWindow(mPrimaryWindow);
                    osg::ref_ptr<osg::Camera> camera = mViewer->getCamera();
                    camera->setGraphicsContext(osgWindow);
                    camera->setViewport(0, 0, resolution.x, resolution.y);

                    mViewer->realize();
                }

                // Setup the main scene and make it the active scene
                mMainScene = mCurrentScene = new CSceneGraph(this);

                // Set up the renderer time pulse
                if (mHasDisplay)
                {
                    const Common::U16 activeFPS = settings->getValue<Common::U16>("Video::ActiveFPS");

                    mTimePulse = Support::SSynchronousScheduler::getInstance()->schedule(Support::FPSToMS(activeFPS), true,
                                                                                        this,
                                                                                        &SRenderer::drawFrame);
                }

                CONSOLE_INFO("Initialized renderer.");

                return 0;
            }

            void SRenderer::setSceneGraph(CSceneGraph* graph)
            {
                if (mCurrentScene)
                {
                    mCurrentScene->setVisible(false);
                }

                mCurrentScene = graph;

                if (mCurrentScene)
                {
                    mCurrentScene->setVisible(true);
                }
            }

            void SRenderer::setResolution(const Support::Dimension2DU& resolution)
            {
             //   al_resize_display(mDisplay, resolution.x(), resolution.y());

               // mIrrlichtDevice->getVideoDriver()->OnResize(resolution);

                GUI::SGUIManager::getInstance()->setResolution(resolution);
               // al_acknowledge_resize(mDisplay);
            }

            osgViewer::Viewer* SRenderer::getViewer(void) const
            {
                return mViewer;
            }

            CSceneGraph* SRenderer::getMainScene(void)
            {
                return mMainScene;
            }

            CSceneGraph* SRenderer::getCurrentScene(void)
            {
                return mCurrentScene;
            }

            CGraphicsWindow* SRenderer::createDisplay(const Support::String& title)
            {
             //   CDisplay* result = new CDisplay(title);

              //  mDisplays.insert(mDisplays.end(), result);
                //return result;
                return nullptr;
            }

            void SRenderer::drawFrame(void)
            {
                PROFILER_BEGIN(Render);

                //al_set_current_opengl_context(mDisplay);

                // FIXME: We can properly hook these into the OSG raster
                mPrimaryWindow->setActiveOpenGLContext();

                mViewer->advance();
                mViewer->updateTraversal();
                mViewer->renderingTraversals();

                mPrimaryWindow->swapBuffers();

                //  GUI::SGUIManager::getInstance()->draw();

               // al_flip_display();

                mPrimaryWindow->processWindowEvents();
                al_inhibit_screensaver(true);

                // Draw the aux displays
                // FIXME: Currently there is buffering problems with multiple displays
                /*
                for (CDisplay* display: mDisplays)
                {
                    al_set_current_opengl_context(display->mDisplay);

                    mVideo->beginScene(true, true, mClearColor);

                    if (mCurrentScene)
                        mSceneManager->drawAll();

                    GUI::SGUIManager::getInstance()->draw();

                    mVideo->endScene();

                    #if !defined(ENGINE_WIN)
                        al_flip_display();
                    #endif
                }
                */

                PROFILER_END(Render);
            }
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
