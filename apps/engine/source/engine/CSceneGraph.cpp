/**
 *  @file CSceneGraph.cpp
 *  @brief The source file implementing the Kiaro::Engine::CSceneGraph class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <engine/SEngineInstance.hpp>
#include <engine/CSceneGraph.hpp>

namespace Kiaro
{
    namespace Engine
    {
        CSceneGraph::CSceneGraph(void)
        {
            mRoot = Kiaro::Engine::SEngineInstance::getPointer()->getSceneManager()->addEmptySceneNode();
        }

        void CSceneGraph::add(irr::scene::ISceneNode *node)
        {
            mRoot->addChild(node);
        }
    } // End NameSpace Engine
} // End NameSpace Kiaro
