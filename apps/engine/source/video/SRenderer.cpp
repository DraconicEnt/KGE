/**
 *  @file SRenderer.cpp
 */

#include <video/SRenderer.hpp>

#include <core/SSettingsRegistry.hpp>
#include <input/SInputListener.hpp>
#include <video/CSceneGraph.hpp>
#include <support/Logging.hpp>

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

        SRenderer::SRenderer(void)
        {
            // Handle Execution Flag
            irr::video::E_DRIVER_TYPE videoDriver = irr::video::EDT_OPENGL;

           // if (mEngineMode == Kiaro::Core::MODE_DEDICATED)
           //     videoDriver = irr::video::EDT_NULL;

            Core::SSettingsRegistry *settings = Core::SSettingsRegistry::getPointer();

            // Init the Input listener
            Input::SInputListener* inputListener = Input::SInputListener::getPointer();

            // Start up Irrlicht
            mIrrlicht = irr::createDevice(videoDriver, settings->getValue<irr::core::dimension2d<Common::U32>>("Resolution"), 32, false, false, false, inputListener);
            mIrrlicht->setWindowCaption(L"Kiaro Game Engine");

            // Grab the scene manager and store it to reduce a function call
            mSceneManager = mIrrlicht->getSceneManager();
            mSceneManager->addCameraSceneNode();

            // Initialize the main scene and set it
            // TODO (Robert MacGregor#9): Only initialize when running as a client.
            mMainScene = new Video::CSceneGraph();
            this->setSceneGraph(mMainScene);

            Support::Logging::write(Support::Logging::MESSAGE_INFO, "SEngineInstance: Irrlicht version is %s.", mIrrlicht->getVersion());
            Support::Logging::write(Support::Logging::MESSAGE_INFO, "SEngineInstance: Initialized renderer.");
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

        }

        void SRenderer::drawFrame(void)
        {
            mVideo->beginScene(true, true);

            mSceneManager->drawAll();

            mVideo->endScene();
        }

/*
        void SRenderer::setClearColor(const Common::ColorRGBA& color)
        {
            mClearColor = color;
        }

        const Common::ColorRGBA& SRenderer::getClearColor(void)
        {
            return mClearColor;
        }
        */
    } // End NameSpace Video
} // End NameSpace Kiaro
