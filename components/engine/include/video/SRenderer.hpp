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

        /**
         *  @brief The SRenderer is a singleton used to control the core aspects of graphical output of the engine.
         *  It provides an interface for windowing as well as code to deal with window events such as resizing.
         */
        class SRenderer
        {
            // Public Members
            public:
                //! The color drawn that is drawn when there is pixel space without anything in it.
                Common::ColorRGBA mClearColor;

                //! A boolean representing whether or not the renderer has a display. It won't have a display in dedicated mode.
                const bool mHasDisplay;

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

                //! The recurring scheduled event representing our frame draw pulse when not in dedicated mode.
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

                /**
                 *  @brief Sets the resolution of the display, if there is one.
                 *  @param resolution The new resolution to use.
                 */
                void setResolution(const Support::Dimension2DU& resolution);

                /**
                 *  @brief Returns the pointer to the internally used Irrlicht device.
                 *  @return A pointer to the internally used Irrlicht device.
                 */
                irr::IrrlichtDevice* getIrrlichtDevice(void) const NOTHROW;

            // Private Methods
            private:
                //! Parameterless constructor.
                SRenderer(void);
                //! Standard destructor.
                ~SRenderer(void);

                /**
                 *  @brief Initializes the GUI subsystem and returns a status code.
                 *  @return A Common::S32 representing the status code of GUI initialization.
                 *  @retval 0 for success. Anything else for failure.
                 */
                Common::S32 initializeGUI(void);

                /**
                 *  @brief Initializes the renderer itself and returns a status code.
                 *  @return A Common::S32 representing the status code of renderer initialization.
                 *  @retval 0 for success. Anything else for failure.
                 */
                Common::S32 initializeRenderer(const Support::Dimension2DU& resolution);

                //! Process all pending events for the game window, such as window resizing.
                void processWindowEvents(void);
        };
    } // End NameSpace Video
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_SRENDERER_HPP_
