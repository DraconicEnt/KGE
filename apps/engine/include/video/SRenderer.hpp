/**
 *  @file SRenderer.hpp
 */

#ifndef _INCLUDE_VIDEO_SRENDERER_HPP_
#define _INCLUDE_VIDEO_SRENDERER_HPP_

#include <irrlicht.h>

#include <allegro5/allegro.h>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CScheduledEvent;
    }

    namespace Video
    {
        class CSceneGraph;

        class SRenderer
        {
            // Private Members
            private:
                irr::IrrlichtDevice* mIrrlichtDevice;
                irr::video::IVideoDriver* mVideo;
                irr::scene::ISceneManager* mSceneManager;

                CSceneGraph* mMainScene;
                CSceneGraph* mCurrentScene;

                ALLEGRO_DISPLAY* mDisplay;
                ALLEGRO_EVENT_QUEUE* mWindowEventQueue;

                Support::CScheduledEvent* mTimePulse;

            // Public Members
            public:
                Common::ColorRGBA mClearColor;

            // Public Methods
            public:
                static SRenderer* getPointer(void);
                static void destroy(void);

                void drawFrame(void);

                void setSceneGraph(CSceneGraph* graph);

            // Private Methods
            private:
                SRenderer(void);
                ~SRenderer(void);

                int initializeGUI(void);
                int initializeRenderer(void);

                void processWindowEvents(void);
        };
    } // End NameSpace Videos
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_SRENDERER_HPP_
