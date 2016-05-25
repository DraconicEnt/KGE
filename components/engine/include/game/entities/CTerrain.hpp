/**
 *  @file CTerrain.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2016 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_CTERRAIN_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_CTERRAIN_HPP_

#include "support/common.hpp"

#include <game/entities/IRigidObject.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    }

    namespace Game
    {
        namespace Entities
        {
            enum TERRAIN_PROPERTY
            {
                TERRAIN_NULL = 0,
                TERRAIN_POSITION = 1,
                TERRAIN_SCALE = 2,
                TERRAIN_TERRAINFILE = 3,
            };

            /**
             *  @brief
             */
            class CTerrain : public Kiaro::Game::Entities::IRigidObject
            {
                // Public Methods
                public:
                    /**
                     *  @brief Constructor accepting a path to a terrain file.
                     *  @param terrainFile The path to the terrain file.
                     */
                    CTerrain(const std::string& terrainFile);

                    /**
                     *  @brief Constructor accepting a CBitStream to unpack terrain
                     *  instantiation data from. This will unpack a terrain file, position,
                     *  etc.
                     *  @param in The input CBitStream to instantiate from.
                     */
                    CTerrain(Kiaro::Support::CBitStream& in);

                    //! Standard destructor.
                    ~CTerrain(void);

                    virtual void packEverything(Kiaro::Support::CBitStream &out) const;
                    virtual void unpack(Kiaro::Support::CBitStream &in);
                    virtual void registerEntity(void);
                    virtual void update(const Kiaro::Common::F32 deltaTimeSeconds);
                    virtual size_t getRequiredMemory(void) const;

                    void setPosition(const Common::Vector3DF& position);


                // Private Members
                private:
                    //! The Irrlicht scene node associated with this terrain.
                    irr::scene::ITerrainSceneNode *mSceneNode;

                    //! The path to the loaded terrain file.
                    std::string mTerrainFile;

                    static std::map<std::string, size_t> sNetworkedProperties;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_CTERRAIN_HPP_
