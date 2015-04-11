/**
 *  @file CSceneGraph.hpp
 *  @brief The include file defining the Kiaro::Engine::CSceneGraph class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_ENGINE_CSCENEGRAPH_HPP_
#define _INCLUDE_ENGINE_CSCENEGRAPH_HPP_

#include <irrlicht.h>

namespace Kiaro
{
    namespace Engine
    {
        class CSceneGraph
        {
            public:
                CSceneGraph(void);

                void add(irr::scene::ISceneNode *node);

            private:
                irr::scene::ISceneNode *mRoot;
        };
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_ENGINE_CSCENEGRAPH_HPP_
