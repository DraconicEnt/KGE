/**
 *  @file CSceneGraph.hpp
 *  @brief The include file defining the Kiaro::Engine::CSceneGraph class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_VIDEO_CSCENEGRAPH_HPP_
#define _INCLUDE_VIDEO_CSCENEGRAPH_HPP_

#include <support/common.hpp>
#include <video/SRenderer.hpp>

#include <irrlicht.h>

#include <game/entities/IEntity.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Video
        {
            /**
             *  @brief The CSceneGraph represents an individual scene to be rendered in 3D space. It is much like
             *  an individual GUI to be rendered, except it is a collection of 3D scene elements.
             */
            class CSceneGraph
            {
            public:
                //! Parameterless constructor.
                CSceneGraph(Engine::Video::SRenderer *renderer);

                //! Standard destructor.
                ~CSceneGraph(void);

                /**
                 *  @brief Adds a renderer scene node to the scene graph, detaching it from its previous
                 *  scene graph.
                 *  @param node A pointer to the node to add to this scene graph.
                 */
                void add(irr::scene::ISceneNode *node);

                void add(Game::Entities::IEntity *entity);

                bool isVisible(void);

                void setVisible(const bool visible);

                // const Kiaro::Common::ColorRGBA &getClearColor(void);
                // void setClearColor(const Kiaro::Common::ColorRGBA &color);

            private:
                //! A pointer to the root scene node.
                irr::scene::ISceneNode *mRoot;
            };
        }
    } // End NameSpace Engine
} // End NameSpace Kiaro
#endif // _INCLUDE_VIDEO_CSCENEGRAPH_HPP_
