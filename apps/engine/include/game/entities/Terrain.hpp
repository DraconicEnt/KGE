/**
 *  @file Terrain.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_TERRAIN_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_TERRAIN_HPP_

#include "engine/Common.hpp"

#include <game/entities/EntityBase.hpp>

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

            class Terrain : public Kiaro::Game::Entities::EntityBase
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
                    std::string mTerrainFile;

                    static std::map<std::string, size_t> sNetworkedProperties;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_TERRAIN_HPP_
