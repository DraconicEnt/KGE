/**
 *  @file CTerrain.hpp
 *  @brief Include file declaring the CTerrain class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
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

    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                /**
                 *  @brief A class representing a terrain block.
                 */
                class CTerrain : public IRigidObject
                {
                    // Public Methods
                    public:
                        /**
                         *  @brief Constructor accepting a path to a terrain file.
                         *  @param terrainFile The path to the terrain file.
                         */
                        CTerrain(const Support::String& terrainFile, const Support::String& textureFile);

                        /**
                         *  @brief Constructor accepting a CBitStream to unpack terrain
                         *  instantiation data from. This will unpack a terrain file, position,
                         *  etc.
                         *  @param in The input CBitStream to instantiate from.
                         */
                        CTerrain(Kiaro::Support::CBitStream& in);

                        //! Standard destructor.
                        ~CTerrain(void);

                        /**
                         *  @brief Packs everything about the terrain block into the CBitStream.
                         *  @param out A reference to the CBitStream to write to.
                         */
                        virtual void packEverything(Kiaro::Support::CBitStream& out) const;

                        /**
                         *  @brief Unpacks new information from the input CBitStream.
                         *  @param in A reference to the CBitStream to unpack from.
                         */
                        virtual void unpack(Kiaro::Support::CBitStream& in);

                        /**
                         *  @brief Registers the terrain block to the game world once prepared.
                         */
                        virtual void registerEntity(void);

                        /**
                         *  @brief Pushes an update to the terrain block. Note that the terrain has no update routine, so this is simply
                         *  a no-op and is simply implemented to satisify virtual method requirements.
                         *  @param deltaTimeSeconds The time in seconds that has passed since the last update.
                         */
                        virtual void update(const Kiaro::Common::F32 deltaTimeSeconds);

                        /**
                         *  @brief Returns the amount of bytes required to pack the terrain block into a CBitStream.
                         *  @return The amount of bytes required to pack the terrain block.
                         */
                        virtual size_t getRequiredMemory(void) const;

                        /**
                         *  @brief Sets the position of the terrain block.
                         *  @param position The new position to go to.
                         */
                        void setPosition(const Common::Vector3DF& position);

                        virtual void registerNetworkedProperties(void);

                    // Private Members
                    private:
                        //! The Irrlicht scene node associated with this terrain.
                        irr::scene::ITerrainSceneNode* mSceneNode;

                        //! The path to the loaded terrain file heightmap.
                        Support::String mTerrainFile;

                        //! The path to the loaded terrain file texture.
                        Support::String mTextureFile;
                };
            } // End Namespace Entities
        } // End Namespace Game
    }
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_CTERRAIN_HPP_
