/**
 *  @file Terrain.cpp
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

#include <game/entities/Terrain.hpp>
#include <game/entities/Types.hpp>
#include <game/ServerSingleton.hpp>

#include <support/BitStream.hpp>

#include <engine/FileReadObject.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            Terrain::Terrain(const std::string &terrainFile) : Kiaro::Game::Entities::EntityBase(Kiaro::Game::Entities::ENTITY_TERRAIN),
            mTerrainFile(terrainFile)
            {
                instantiate();
            }

            Terrain::Terrain(Kiaro::Support::BitStream &in) : Kiaro::Game::Entities::EntityBase(Kiaro::Game::Entities::ENTITY_TERRAIN)
            {
                unpackInitialization(in);
            }

            Terrain::~Terrain(void)
            {

            }

            void Terrain::packUpdate(Kiaro::Support::BitStream &out)
            {
                const Kiaro::Common::Vector3DF &position = mSceneNode->getPosition();

            }

            void Terrain::unpackUpdate(Kiaro::Support::BitStream &in)
            {

            }

            void Terrain::packInitialization(Kiaro::Support::BitStream &out)
            {
                const Kiaro::Common::Vector3DF &position = mSceneNode->getPosition();

                out.writeF32(position.X);
                out.writeF32(position.Y);
                out.writeF32(position.Z);
                out.writeString(mTerrainFile);
            }

            void Terrain::unpackInitialization(Kiaro::Support::BitStream &in)
            {
                mTerrainFile = in.readString();

                instantiate();

                if (mSceneNode)
                {
                    Kiaro::Common::Vector3DF position;
                    position.Z = in.readF32();
                    position.Y = in.readF32();
                    position.X = in.readF32();

                    mSceneNode->setPosition(position);
                }
            }

            void Terrain::instantiate(void)
            {
                Kiaro::Engine::FileReadObject fileHandle(mTerrainFile);

                irr::IrrlichtDevice *irrlichtDevice = Kiaro::Engine::CoreSingleton::getPointer()->getIrrlichtDevice();
                irr::scene::ITerrainSceneNode *terrain = irrlichtDevice->getSceneManager()->addTerrainSceneNode(&fileHandle);

                if (terrain)
                {
                    terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);

                    mSceneNode = terrain;
                }
                else
                    std::cerr << "Terrain: Failed to instantiate using '" << mTerrainFile << "'" << std::endl;
            }

            void Terrain::update(const Kiaro::Common::F32 &deltaTimeSeconds)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
