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
    namespace Engine
    {
        namespace Video
        {
            CSceneGraph::CSceneGraph(Engine::Video::SRenderer* renderer)
            {
                mGroup = new osg::Group();
            }

            CSceneGraph::~CSceneGraph(void)
            {
                CONSOLE_INFO("Destroyed a scene graph.");
            }

            void CSceneGraph::add(osg::Node* node)
            {
                mGroup->addChild(node);
            }

            void CSceneGraph::add(Game::Entities::IEntity* entity)
            {
                for (osg::Node* node: entity->mSceneNodes)
                {
                    this->add(node);
                }
            }

            bool CSceneGraph::isVisible(void)
            {
                // FIXME: Implement correctly
                return true;
            }

            void CSceneGraph::setVisible(const bool visible)
            {
                // FIXME: Implement correctly
            }
        } // End NameSpace Engine
    }
} // End NameSpace Kiaro
