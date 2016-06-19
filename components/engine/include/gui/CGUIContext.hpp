/*
 *
 */

#ifndef _INCLUDE_KIARO_VIDEO_CGUICONTEXT_HPP_
#define _INCLUDE_KIARO_VIDEO_CGUICONTEXT_HPP_

#include <support/types.hpp>
#include <support/UnorderedMap.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

namespace Kiaro
{
    namespace Engine
    {
        namespace GUI
        {
            class SGUIManager;

            class CGUIContext
            {
                public:
                    CGUIContext(const Support::String& name);
                    ~CGUIContext(void);

                    void setCursorPosition(const Support::Vector2DF& position);

                    CEGUI::FrameWindow *showDialog(const Support::String &name);

                    void displayMessageBoxOK(const Support::String &title, const Support::String &body);

                    void setCursorVisible(bool visible);

                private:
                    CEGUI::GUIContext* mContext;

                    const Support::String mName;

                    //! The root of the GUI system.
                    CEGUI::Window *mRoot;

                    //! The parent of whatever GUI we're displaying.
                    CEGUI::Window *mGUI;

                    //! The current dialog we're displaying.
                    CEGUI::FrameWindow *mDialog;

                    Support::UnorderedMap<Support::String, CEGUI::FrameWindow*> mDialogs;
                    Support::UnorderedMap<Support::String, CEGUI::Window*> mGUIs;
            };
        } // End NameSpace GUI
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif //_INCLUDE_KIARO_VIDEO_CGUICONTEXT_HPP_
