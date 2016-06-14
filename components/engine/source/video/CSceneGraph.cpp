/**
 *  @file CSceneGraph.cpp
 *  @brief The source file implementing the Kiaro::Engine::CSceneGraph class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <core/SEngineInstance.hpp>

#include <video/CSceneGraph.hpp>

namespace Kiaro
{
    namespace Video
    {
        CSceneGraph::CSceneGraph(Video::SRenderer* renderer)
        {
            mRoot = renderer->getIrrlichtDevice()->getSceneManager()->addEmptySceneNode();
        }

        CSceneGraph::~CSceneGraph(void)
        {
            assert(mRoot);

            mRoot->removeAll();
            mRoot->remove();
            mRoot = nullptr;
            CONSOLE_INFO("Destroyed a scene graph.");
        }

        void CSceneGraph::add(irr::scene::ISceneNode* node)
        {
            mRoot->addChild(node);
        }

        void CSceneGraph::add(Game::Entities::IEntity* entity)
        {
            for (irr::scene::ISceneNode* node: entity->mSceneNodes)
                this->add(node);
        }

        bool CSceneGraph::isVisible(void)
        {
            return mRoot->isVisible();
        }

        void CSceneGraph::setVisible(const bool visible)
        {
            mRoot->setVisible(visible);
        }

        /*
                const Common::ColorRGBA &CSceneGraph::getClearColor(void)
                {
                    return mClearColor;
                }

                void CSceneGraph::setClearColor(const Common::ColorRGBA &color)
                {
                    mClearColor = color;
                }
                */

    } // End NameSpace Engine
} // End NameSpace Kiaro
