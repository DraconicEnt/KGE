/**
 *  @file COSGWindow.hpp
 *  @brief Include file declaring the COSGWindow class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_ENGINE_VIDEO_COSGWINDOW_HPP_
#define _INCLUDE_ENGINE_VIDEO_COSGWINDOW_HPP_

#include <video/CGraphicsWindow.hpp>
#include <osgViewer/GraphicsWindow>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            class COSGWindow : public osgViewer::GraphicsWindow
            {
                private:
                    bool            mValid;
                    bool            mRealized;
                    bool            mOwnsWindow;

                    virtual ~COSGWindow();

                    CGraphicsWindow* mWindow;

                public:
                    COSGWindow(CGraphicsWindow* window);

                    virtual bool isSameKindAs(const Object* object) const { return dynamic_cast<const COSGWindow*>(object)!=0; }
                    virtual const char* libraryName() const { return "osgViewer"; }
                    virtual const char* className() const { return "COSGWindow"; }

                    virtual bool valid() const { return mValid; }

                    virtual bool isRealizedImplementation() const { return mRealized; }

                    virtual void grabFocus() {}

                    virtual void grabFocusIfPointerInWindow() {}

                    virtual bool realizeImplementation();

                    virtual void closeImplementation();

                    virtual bool makeCurrentImplementation();

                    virtual bool releaseContextImplementation();

                    virtual void swapBuffersImplementation();

                    virtual void setSyncToVBlank(bool on);

                    virtual bool setWindowDecorationImplementation(bool flag);

                    virtual void raiseWindow();

                    virtual bool setWindowRectangleImplementation(Common::S32 x, Common::S32 y, Common::S32 width, Common::S32 height);

                    virtual void setWindowName(const Support::String& name);

                    virtual void setCursor(MouseCursor cursor);

                    struct WindowData : public osg::Referenced
                    {
                        WindowData(CGraphicsWindow *window) : mWindow(window) { }

                        CGraphicsWindow *mWindow;
                    };
            };
        }
    }
}

#endif // _INCLUDE_ENGINE_VIDEO_COSGWINDOW_HPP_
