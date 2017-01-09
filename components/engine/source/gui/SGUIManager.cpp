/**
 *  @file SGUIManager.cpp
 *  @brief Source file implementing the SGUIManager singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <gui/SGUIManager.hpp>
#include <gui/CGUIContext.hpp>

#include <support/Console.hpp>
#include <video/SRenderer.hpp>
#include <filesystem/SResourceProvider.hpp>
#include <support/SSettingsRegistry.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace GUI
        {
            void SGUIManager::draw(void)
            {
              //  CEGUI::System::getSingleton().renderAllGUIContexts();
            }

            void SGUIManager::setResolution(const Support::Dimension2DU &resolution)
            {
               // CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(resolution.Width, resolution.Height));
            }

            SGUIManager::SGUIManager(void)
            {
                CONSOLE_INFO("Initializing GUI subsystem.");

               // mPlatform = new MyGUI::OpenGLPlatform();
              //  mPlatform->initialise(nullptr);

              //  mMyGUI = new MyGUI::Gui();
              //  mMyGUI->initialise("core.xml");

                // Make sure the resolution is right
                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getInstance();
                Support::Dimension2DU resolution = settings->getValue<Support::Dimension2DU>(
                        "Video::Resolution");
                this->setResolution(resolution);

                CONSOLE_INFO("Initialized the GUI system.");
            }

            SGUIManager::~SGUIManager(void)
            {

            }

            /*
            CGUIContext* SGUIManager::createContext(const Support::String& name)
            {
                if (mContexts.find(name) != mContexts.end())
                {
                    CONSOLE_ERRORF("Attempted to create a context with the same name as an existing one: %s", name.data());
                    return mContexts[name];
                }

                CONSOLE_INFOF("Creating GUI context: %s", name.data());

                CGUIContext* result = new CGUIContext(name);
                mContexts[name] = result;

                return result;
            }
            */

            /*
            CEGUI::FrameWindow* SGUIManager::loadDialog(const Support::String &name)
            {
                CEGUI::FrameWindow* result = nullptr;

                if (mDialogs.find(name) != mDialogs.end())
                    return static_cast<CEGUI::FrameWindow*>(mDialogs[name]->clone(true));

                Support::String fileName = "layouts/";
                fileName += name;
                fileName += ".layout";

                CONSOLE_INFOF("Loading layout for %s: %s", name.data(), fileName.data());


                // Load the GUI's
              //  result = static_cast<CEGUI::FrameWindow *>(windowManager.loadLayoutFromFile(fileName.data())->getChild("Window"));
              //  result->hide();

                mDialogs[name] = result;

                return result;
            }
            */

            /*
            CGUIContext* SGUIManager::getContext(const Support::String& name)
            {
                auto searchResult = mContexts.find(name);

                if (searchResult != mContexts.end())
                    return (*searchResult).second;

                CONSOLE_ERRORF("No such GUI context: %s", name.data());
                return nullptr;
            }
            */

            bool SGUIManager::loadGUI(const Support::String &name)
            {
                return false;
            }
        }
    }
}
