/*
 *
 */

#include <gui/CGUIContext.hpp>

#include <gui/SGUIManager.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace GUI
        {
            CGUIContext::CGUIContext(const Support::String& name) : mGUI(nullptr), mDialog(nullptr), mContext(nullptr), mName(name)
            {
                CEGUI::System &cegui = CEGUI::System::getSingleton();

                if (name == "main")
                    mContext = &cegui.getDefaultGUIContext();
                else
                {
                    CEGUI::IrrlichtRenderer* irrlichtRenderer = static_cast<CEGUI::IrrlichtRenderer*>(cegui.getRenderer());

                    CEGUI::TextureTarget* rtt = irrlichtRenderer->createTextureTarget();

                    // FIXME: Accept RTT size
                    rtt->declareRenderSize(CEGUI::Sizef(640, 480));

                    mContext = &cegui.createGUIContext(*rtt);
                }

                // Initialize the context
                mContext->setDefaultFont("DejaVuSans-10");
                mContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
                mContext->getMouseCursor().setImage(mContext->getMouseCursor().getDefaultImage());

                CEGUI::WindowManager &windowManager = CEGUI::WindowManager::getSingleton();
                mRoot = windowManager.createWindow("DefaultWindow", "root");
                mContext->setRootWindow(mRoot);

                // Now finally default the cursor to invisible
                this->setCursorVisible(false);
            }

            CEGUI::FrameWindow* CGUIContext::showDialog(const Support::String &name)
            {
                if (mDialog)
                    mDialog->hide();

                SGUIManager* manager = SGUIManager::getPointer();

                // First, do we have a dialog in-memory already?
                auto searchResult = mDialogs.find(name);

                if (searchResult != mDialogs.end())
                    mDialog = (*searchResult).second;
                else
                {
                    CEGUI::FrameWindow* result = manager->loadDialog(name);

                    if (!result)
                        return nullptr;

                    mDialog = result;
                }

                mRoot->addChild(mDialog);

                mDialog->show();
                mDialog->moveToFront();
                mDialog->setModalState(true);

                return mDialog;
            }

            void CGUIContext::displayMessageBoxOK(const Support::String &title, const Support::String &body)
            {
                try
                {
                    CEGUI::FrameWindow *dialog = this->showDialog("messageBoxOK");

                    if (!dialog)
                        return;

                    dialog->setText(title.data());

                    static_cast<CEGUI::MultiLineEditbox *>(dialog->getChild("Body"))->setText(body.data());
                    CEGUI::ButtonBase *button = static_cast<CEGUI::ButtonBase *>(dialog->getChild("Button"));
                }
                catch (CEGUI::UnknownObjectException &e)
                {
                    // FIXME: Deal with this in some way
                }
            }

            CGUIContext::~CGUIContext(void)
            {
                CEGUI::System &cegui = CEGUI::System::getSingleton();
                cegui.destroyGUIContext(*mContext);
            }

            void CGUIContext::setCursorPosition(const Support::Vector2DF& position)
            {
                mContext->injectMousePosition(position.X, position.Y);
            }

            void CGUIContext::setCursorVisible(bool visible)
            {
                mContext->getMouseCursor().setVisible(visible);
            }
        } // End NameSpace GUI
    } // End NameSpace Engine
} // End NameSpace Kiaro

