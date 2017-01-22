/**
 *  @file CGUIContext.hpp
 *  @brief Include file for the CGUIContext class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_GUI_CGUICONTEXT_HPP_
#define _INCLUDE_KIARO_ENGINE_GUI_CGUICONTEXT_HPP_

#include <support/types.hpp>
#include <support/UnorderedMap.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace GUI
        {
            /**
             * @brief A GUI context essentially another plane of existence for GUI's to operate in. This is useful
             * to operate sub-GUI's, perhaps for rendering on 3D objects in the game world itself.
             * @details The context rendering to the main frame buffer will always be called "main". All other
             * contexts created will always render to a texture when operating, and the active state should be
             * set accordingly to save GPU resources.
             */
            class CGUIContext
            {
                // Private Members
                private:
                    //! The name of this GUI context.
                    const Support::String mName;

                    //! A pointer to the CEGUI context we're dealing with internally.
                   // CEGUI::GUIContext* mContext;

                    //! The root of the GUI system.
                   // CEGUI::Window* mRoot;

                    //! The parent of whatever GUI we're displaying.
                   // CEGUI::Window* mGUI;

                    //! The current dialog we're displaying.
                   // CEGUI::FrameWindow* mDialog;

                    //! A mapping of dialog names to CEGUI frame window of the dialog.
                 //   Support::UnorderedMap<Support::String, CEGUI::FrameWindow*> mDialogs;

                    //! A mapping of GUI names to the CEGUI window containing the entire GUI.
                //    Support::UnorderedMap<Support::String, CEGUI::Window*> mGUIs;

                // Public Methods
                public:
                    /**
                     * @brief Constructor accepting a context name.
                     * @param name The name of the context. This should be unique.
                     */
                    CGUIContext(const Support::String& name);

                    //! Standard destructor.
                    ~CGUIContext(void);

                    /**
                     * @brief Sets the position of the cursor in this context.
                     * @param position The position to put the GUI context's cursor at.
                     */
                    void setCursorPosition(const Support::Vector2DF& position);

                    /**
                     * @brief Pushes a dialog to the screen for the user to see, identified by name.
                     * @details Only one dialog can be presented at a time: all dialogs will override each other. Dialogs
                     * are always rendering above the current GUI and are always modal. If the dialog currently isn't
                     * loaded, the following CEGUI layout XML file is parsed to create it: layouts/<name>.layout
                     * @param name The name of the dialog to present to the user.
                     * @return The frame window of the dialog to be used for setting whatever GUI state is
                     * desired for this dialog.
                     */
                  //  CEGUI::FrameWindow* showDialog(const Support::String &name);

                    /**
                     * @brief Helper method to display a generic message box with some message.
                     * @param title The title of the message box.
                     * @param body The text body to display.
                     */
                    void displayMessageBoxOK(const Support::String &title, const Support::String &body);

                    /**
                     * @brief Sets whether or not the cursor should be rendered on this context.
                     * @param visible Whether or not the cursor should be visible.
                     */
                    void setCursorVisible(bool visible);
            };
        } // End NameSpace GUI
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif //_INCLUDE_KIARO_ENGINE_GUI_CGUICONTEXT_HPP_
