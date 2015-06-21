/**
 *  @file SRenderer.cpp
 */

#include <video/SRenderer.hpp>

namespace Kiaro
{
    namespace Video
    {
        static SRenderer *sInstance = NULL;

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
            // TODO (Robert MacGregor#9): Load resolution, etc from settings registry

            // Start up Irrlicht
            mIrrlicht = irr::createDevice(irr::video::EDT_OPENGL,
            //irr::core::dimension2d<Common::U32>(640, 480), 32, false, false, false, inputListener);
            irr::core::dimension2d<Common::U32>(640, 480), 32, false, false, false, NULL);

            mIrrlicht->setWindowCaption(L"Kiaro Game Engine");

            // Grab the scene manager and store it to reduce a function call
            mSceneManager = mIrrlicht->getSceneManager();

            std::cout << "SRenderer: Irrlicht version is " << mIrrlicht->getVersion() << std::endl;
            std::cout << "SRenderer: Initialized renderer. " << std::endl;
        }

        SRenderer::~SRenderer(void)
        {

        }

        void SRenderer::drawFrame(void)
        {
            mVideo->beginScene(true, true, mClearColor);

            mSceneManager->drawAll();

            mVideo->endScene();
        }

        void SRenderer::setClearColor(const Common::ColorRGBA& color)
        {
            mClearColor = color;
        }

        const Common::ColorRGBA& SRenderer::getClearColor(void)
        {
            return mClearColor;
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
