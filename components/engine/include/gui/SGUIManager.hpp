/**
 *  @file SGUIManager.hpp
 *  @brief Include file for the SGUIManager singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_ENGINE_VIDEO_SGUIMANAGER_HPP_
#define _INCLUDE_ENGINE_VIDEO_SGUIMANAGER_HPP_

#include <support/types.hpp>
#include <support/UnorderedMap.hpp>
#include <support/ISingleton.hpp>

#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

#include <gui/CGUIContext.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace GUI
        {
            /**
             * @brief The SGUIManager provides access to GUI context creation and dialog/GUI instantiation
             */
            class SGUIManager : public Support::ISingleton<SGUIManager>
            {
                // Private Members
                private:
                    //! A mapping of dialog names to CEGUI frame windows representing the main visible dialog window.
                    Support::UnorderedMap<Support::String, CEGUI::FrameWindow*> mDialogs;

                    //! A mapping of GUI names to CEGUI windows representing the entire GUI to render.
                    Support::UnorderedMap<Support::String, CEGUI::Window*> mGUIs;

                    //! A mapping of context names to instantiated GUI contexts.
                    Support::UnorderedMap<Support::String, CGUIContext*> mContexts;

                public:
                    void draw(void);

                    void setResolution(const Support::Dimension2DU& resolution);

                    /**
                     * @brief Loads a dialog with the given name. It is loaded from the CEGUI XML layout file
                     * with the following pathname: "layouts/<name>.layout". If the dialog is already loaded,
                     * a deep copy is returned.
                     * @param name The name of the dialog to load.
                     * @return A pointer to the frame window representing the uppermost visible window of the dialog.
                     * @retval nullptr Returned when no dialog could be found with the given name -- no such layout
                     * file could be found.
                     */
                    CEGUI::FrameWindow* loadDialog(const Support::String &name);

                    bool loadGUI(const Support::String &name);

                    /**
                     * @brief Creates a new GUI context with the given name.
                     * @param name The name of the GUI context to use. This name should be unique.
                     * @return A pointer to the created context. This context isn't guaranteed to be unique; if thew
                     * specified context name is already in use then the existing context is returned.
                     * @retval nullptr Returned when an error has occurred in creation.
                     */
                    CGUIContext* createContext(const Support::String& name);

                    /**
                     * @brief Returns a GUI context by name.
                     * @param name The name of the context to search for.
                     * @return A pointer to the GUI context that was found.
                     * @retval nullptr Returned when no such context exists by the given name.
                     */
                    CGUIContext* getContext(const Support::String& name);

                // Protected Methods
                protected:
                    //! Parameter-less constructor.
                    SGUIManager(void);
                    //! Standard destructor.
                    ~SGUIManager(void);
            };
        }
    } // End NameSpace Video
} // End NameSpace Kiaro
#endif // _INCLUDE_ENGINE_VIDEO_SGUIMANAGER_HPP_
