/**
 *  @file SRenderer.hpp
 */

#ifndef _INCLUDE_VIDEO_SRENDERER_HPP_
#define _INCLUDE_VIDEO_SRENDERER_HPP_

#include <irrlicht.h>

#include <allegro5/allegro.h>

#include <support/common.hpp>
#include <support/support.hpp>

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
            // Public Members
            public:
                //! The color drawn that is drawn when there is pixel space without anything in it.
                Common::ColorRGBA mClearColor;

            // Private Members
            private:
                //! The Irrlichr context.
                irr::IrrlichtDevice* mIrrlichtDevice;
                //! Irrlicht's video driver.
                irr::video::IVideoDriver* mVideo;
                //! Irrlicht's scene manager.
                irr::scene::ISceneManager* mSceneManager;

                //! The primary scene used to render the main game.
                CSceneGraph* mMainScene;
                //! The scene to render.
                CSceneGraph* mCurrentScene;

                //! A pointer to the Allegro display we are using.
                ALLEGRO_DISPLAY* mDisplay;
                //! A pointer to the event queue for our Allegro display.
                ALLEGRO_EVENT_QUEUE* mWindowEventQueue;

                Support::CScheduledEvent* mTimePulse;

            // Public Methods
            public:
                /**
                 *  @brief Returns a pointer to the current SRender instance, creating a new one
                 *  if necessary.
                 *  @return A pointer to the current SRender singleton instance.
                 */
                static SRenderer* getPointer(void);
                //! Destroys the SRenderer singleton instance.
                static void destroy(void);

                //! Draws a single frame to the Allegro display.
                void drawFrame(void);

                /**
                 *  @brief Sets the scene graph to render.
                 *  @param graph A pointer to the scene graph to be rendering. If this is NULL,
                 *  then no scene is rendered.
                 */
                void setSceneGraph(CSceneGraph* graph);

                void setResolution(const Support::Dimension2DU& resolution);

            // Private Methods
            private:
                //! Parameterless constructor.
                SRenderer(void);
                //! Standard destructor.
                ~SRenderer(void);

                int initializeGUI(void);
                int initializeRenderer(const Support::Dimension2DU& resolution);

                void processWindowEvents(void);
        };
    } // End NameSpace Videos
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_SRENDERER_HPP_
