/*
 *
 */

#ifndef _INCLUDE_VIDEO_SGUIMANAGER_HPP_
#define _INCLUDE_VIDEO_SGUIMANAGER_HPP_

#include <support/types.hpp>
#include <support/UnorderedMap.hpp>

#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

namespace Kiaro
{
    namespace Engine
    {
        namespace GUI
        {
            class CGUIContext;

            class SGUIManager
            {
                public:
                    static SGUIManager *getPointer(void);

                    static void destroy(void);

                    void draw(void);

                    void setResolution(const Support::Dimension2DU &resolution);

                    CEGUI::FrameWindow* loadDialog(const Support::String &name);

                    bool loadGUI(const Support::String &name);

                    CGUIContext* createContext(const Support::String& name);

                    CGUIContext* getContext(const Support::String& name);

                private:
                    SGUIManager(void);

                    Support::UnorderedMap<Support::String, CEGUI::FrameWindow *> mDialogs;
                    Support::UnorderedMap<Support::String, CEGUI::Window *> mGUIs;

                    Support::UnorderedMap<Support::String, CGUIContext*> mContexts;
            };
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_SGUIMANAGER_HPP_
