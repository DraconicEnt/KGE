/**
 *  @file COSGWindow.cpp
 *  @brief Source file imlement COSGWindow.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <video/COSGWindow.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            COSGWindow::COSGWindow(CGraphicsWindow* window) : mWindow(window)
            {
                _traits = new osg::GraphicsContext::Traits;
                _traits->width = window->mWindowParameters.mResolution.x;
                _traits->height = window->mWindowParameters.mResolution.y;
                _traits->x = 0;
                _traits->y = 0;
                _traits->vsync = false;
                _traits->windowDecoration = true;
                _traits->screenNum = 0;

                _traits->red = 8;
                _traits->green = 8;
                _traits->blue = 8;
                _traits->alpha = 0;
                _traits->depth = 24;
                _traits->stencil = 8;
                _traits->doubleBuffer = true;
                _traits->windowName = window->mWindowParameters.mTitle;
                _traits->inheritedWindowData = new WindowData(window);

                mValid = true;
                mRealized = true;

                // Initialize the OSG state
                if(valid())
                {
                    setState(new osg::State);
                    getState()->setGraphicsContext(this);

                    if(_traits.valid() && _traits->sharedContext.valid())
                    {
                        getState()->setContextID(_traits->sharedContext->getState()->getContextID());
                        incrementContextIDUsageCount(getState()->getContextID());
                    }
                    else
                    {
                        getState()->setContextID(osg::GraphicsContext::createNewContextID());
                    }
                }
            }

            COSGWindow::~COSGWindow()
            {

            }

            bool COSGWindow::realizeImplementation()
            {
                return true;
            }

            void COSGWindow::closeImplementation()
            {

            }

            bool COSGWindow::makeCurrentImplementation()
            {
                mWindow->setActiveOpenGLContext();
                return true;
            }

            bool COSGWindow::releaseContextImplementation()
            {
                return true;
            }

            void COSGWindow::swapBuffersImplementation()
            {
                mWindow->swapBuffers();
            }

            void COSGWindow::setSyncToVBlank(bool on)
            {

            }

            bool COSGWindow::setWindowDecorationImplementation(bool flag)
            {
                return true;
            }

            void COSGWindow::raiseWindow()
            {

            }

            bool COSGWindow::setWindowRectangleImplementation(Common::S32 x, Common::S32 y, Common::S32 width, Common::S32 height)
            {
                return true;
            }

            void COSGWindow::setWindowName(const Support::String& name)
            {
                mWindow->setTitle(name);
            }

            void COSGWindow::setCursor(MouseCursor cursor)
            {

            }
        }
    }
}
