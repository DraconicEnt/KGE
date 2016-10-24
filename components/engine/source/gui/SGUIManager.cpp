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
                CEGUI::System::getSingleton().renderAllGUIContexts();
            }

            void SGUIManager::setResolution(const Support::Dimension2DU &resolution)
            {
                CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(resolution.Width, resolution.Height));
            }

            SGUIManager::SGUIManager(void)
            {
                CONSOLE_INFO("Initializing GUI subsystem.");
                irr::IrrlichtDevice *irrlicht = Video::SRenderer::getInstance()->getIrrlichtDevice();

                try
                {
                    // We don't need the OS cursor
                    CEGUI::IrrlichtRenderer &renderer = CEGUI::IrrlichtRenderer::create(*irrlicht);

                    FileSystem::SResourceProvider *resourceProvider = FileSystem::SResourceProvider::getPointer();
                    CEGUI::System::create(renderer, resourceProvider, nullptr, nullptr, nullptr, "", "log.txt");

                    resourceProvider->setResourceGroupDirectory("fonts", "fonts/");
                    resourceProvider->setResourceGroupDirectory("ui", "ui/");
                    CEGUI::System &cegui = CEGUI::System::getSingleton();
                    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "ui");
                    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font", "fonts");

                    // Create the main context
                    this->createContext("main");
                }
                catch (CEGUI::InvalidRequestException &e)
                {
                    CONSOLE_ERROR("Failed to initialize the GUI System. Reason:\n%s", e.what());
                    return;
                }

                // Make sure the resolution is right
                Support::SSettingsRegistry *settings = Support::SSettingsRegistry::getInstance();
                irr::core::dimension2d<Common::U32> resolution = settings->getValue<irr::core::dimension2d<Common::U32>>(
                        "Video::Resolution");
                this->setResolution(resolution);

                CONSOLE_INFO("Initialized the GUI system.");
            }

            SGUIManager::~SGUIManager(void)
            {

            }

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

            CEGUI::FrameWindow* SGUIManager::loadDialog(const Support::String &name)
            {
                CEGUI::FrameWindow* result = nullptr;

                if (mDialogs.find(name) != mDialogs.end())
                    return static_cast<CEGUI::FrameWindow*>(mDialogs[name]->clone(true));

                Support::String fileName = "layouts/";
                fileName += name;
                fileName += ".layout";

                CONSOLE_INFOF("Loading layout for %s: %s", name.data(), fileName.data());

                CEGUI::WindowManager &windowManager = CEGUI::WindowManager::getSingleton();

                // Load the GUI's
                result = static_cast<CEGUI::FrameWindow *>(windowManager.loadLayoutFromFile(fileName.data())->getChild("Window"));
                result->hide();

                mDialogs[name] = result;

                return result;
            }

            CGUIContext* SGUIManager::getContext(const Support::String& name)
            {
                auto searchResult = mContexts.find(name);

                if (searchResult != mContexts.end())
                    return (*searchResult).second;

                CONSOLE_ERRORF("No such GUI context: %s", name.data());
                return nullptr;
            }

            bool SGUIManager::loadGUI(const Support::String &name)
            {
                return false;
            }
        }
    }
}
