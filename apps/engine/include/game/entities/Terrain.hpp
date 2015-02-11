/**
 *  @file Terrain.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_TERRAIN_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_TERRAIN_HPP_

#include "engine/Common.hpp"

#include <game/entities/RigidObjectBase.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        class BitStream;
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

            class Terrain : public Kiaro::Game::Entities::RigidObjectBase
            {
                // Public Methods
                public:
                    Terrain(const std::string &terrainFile);
                    Terrain(Kiaro::Support::BitStream &in);

                    ~Terrain(void);

                    virtual void packUpdate(Kiaro::Support::BitStream &out);
                    virtual void unpackUpdate(Kiaro::Support::BitStream &in);
                    virtual void packInitialization(Kiaro::Support::BitStream &out);
                    virtual void unpackInitialization(Kiaro::Support::BitStream &in);
                    virtual void instantiate(void);
                    virtual void update(const Kiaro::Common::F32 &deltaTimeSeconds);

                // Private Members
                private:
                    irr::scene::ITerrainSceneNode *mSceneNode;

                    std::string mTerrainFile;

                    static std::map<std::string, size_t> sNetworkedProperties;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_TERRAIN_HPP_
