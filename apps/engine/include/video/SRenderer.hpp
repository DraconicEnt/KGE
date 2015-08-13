/**
 *  @file SRenderer.hpp
 */

#ifndef _INCLUDE_VIDEO_SRENDERER_HPP_
#define _INCLUDE_VIDEO_SRENDERER_HPP_

#include <irrlicht.h>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Video
    {
        class CSceneGraph;

        class SRenderer
        {
            // Private Members
            private:
                irr::IrrlichtDevice* mIrrlicht;
                irr::video::IVideoDriver* mVideo;
                irr::scene::ISceneManager* mSceneManager;

                CSceneGraph* mMainScene;
                CSceneGraph* mCurrentScene;

               // Common::ColorRGBA mClearColor;

            // Public Methods
            public:
                static SRenderer* getPointer(void);
                static void destroy(void);

                void drawFrame(void);

                void setSceneGraph(CSceneGraph* graph);
               // void setClearColor(const Common::ColorRGBA& color);
               // const Common::ColorRGBA& getClearColor(void);

            // Private Methods
            private:
                SRenderer(void);
                ~SRenderer(void);
        };
    } // End NameSpace Videos
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_SRENDERER_HPP_
