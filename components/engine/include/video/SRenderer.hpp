/**
 *  @file SRenderer.hpp
 *  @brief Include file declaring the SRenderer singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_VIDEO_SRENDERER_HPP_
#define _INCLUDE_VIDEO_SRENDERER_HPP_

#include <osgViewer/Viewer>
#include <allegro5/allegro.h>

#include <support/common.hpp>
#include <support/support.hpp>
#include <support/ISingleton.hpp>

#include <video/CGraphicsWindow.hpp>
#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CScheduledEvent;
    }

    namespace Engine
    {
        namespace Video
        {
            class CSceneGraph;

            /**
             *  @brief The SRenderer is a singleton used to control the core aspects of graphical output of the engine.
             *  It provides an interface for windowing as well as code to deal with window events such as resizing.
             */
            class SRenderer : public Support::ISingleton<SRenderer>
            {
                // Public Members
                public:
                    //! The color drawn that is drawn when there is pixel space without anything in it.
                    Support::ColorRGBA mClearColor;

                    //! A boolean representing whether or not the renderer has a display. It won't have a display in dedicated mode.
                    const bool mHasDisplay;

                // Private Members
                private:
                    //! The OSG viewer to use.
                    osgViewer::Viewer* mViewer;

                    //! The primary scene used to render the main game.
                    CSceneGraph* mMainScene;
                    //! The scene to render.
                    CSceneGraph* mCurrentScene;

                    //! The recurring scheduled event representing our frame draw pulse when not in dedicated mode.
                    Support::CScheduledEvent* mTimePulse;

                    //! A set of auxilliary displays to also draw to.
                    Support::UnorderedSet<CGraphicsWindow*> mDisplays;

                    //! The primary graphics window.
                    CGraphicsWindow* mPrimaryWindow;

                // Public Methods
                public:
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
                    osgViewer::Viewer* getViewer(void) const NOTHROW;

                    /**
                     *  @brief Returns a pointer to the main scene graph. This is where the normal game sim will operate in.
                     *  @return A pointer to the main CSceneGraph instance.
                     */
                    CSceneGraph* getMainScene(void);

                    /**
                     *  @brief Returns a pointer to the currently rendered scene.
                     *  @return A pointer to the currently rendered scene.
                     */
                    CSceneGraph* getCurrentScene(void);

                    /**
                     *  @brief Creates an auxilliary display, particularly useful when we're running on a multi-display PC.
                     *  @param title The window title to use.
                     *  @return A pointer to the display that was created.
                     *  @warning Auxilliary displays do not function correctly right now. The way the graphics libraries we are using
                     *  are written (particularly CEGUI), they may not work without switching out for a more compatible library.
                     */
                    CGraphicsWindow* createDisplay(const Support::String& title);

                // Private Methods
                private:
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

                // Protected Methods
                protected:
                    //! Parameter-less constructor.
                    SRenderer(void);

                    //! Standard destructor.
                    ~SRenderer(void);
            };
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_SRENDERER_HPP_
