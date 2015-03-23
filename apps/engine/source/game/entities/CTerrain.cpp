/**
 *  @file CTerrain.cpp
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

#include <game/entities/CTerrain.hpp>
#include <game/entities/types.hpp>

#include <network/SServer.hpp>

#include <support/CBitStream.hpp>

#include <engine/CFileReader.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            CTerrain::CTerrain(const std::string &terrainFile) : Kiaro::Game::Entities::IRigidObject(Kiaro::Game::Entities::ENTITY_TERRAIN),
            mTerrainFile(terrainFile)
            {
                instantiate();
            }

            CTerrain::CTerrain(Kiaro::Support::CBitStream &in) : Kiaro::Game::Entities::IRigidObject(Kiaro::Game::Entities::ENTITY_TERRAIN)
            {
                unpackInitialization(in);
            }

            CTerrain::~CTerrain(void)
            {

            }

            void CTerrain::packUpdate(Kiaro::Support::CBitStream &out)
            {


            }

            void CTerrain::unpackUpdate(Kiaro::Support::CBitStream &in)
            {

            }

            void CTerrain::packInitialization(Kiaro::Support::CBitStream &out)
            {
                const Kiaro::Common::Vector3DF &position = mSceneNode->getPosition();

                out.write<Kiaro::Common::F32>(position.X);
                out.write<Kiaro::Common::F32>(position.Y);
                out.write<Kiaro::Common::F32>(position.Z);
                out.write(mTerrainFile);
            }

            void CTerrain::unpackInitialization(Kiaro::Support::CBitStream &in)
            {
                mTerrainFile = in.readString();

                instantiate();

                if (mSceneNode)
                {
                    Kiaro::Common::Vector3DF position;
                    position.Z = in.read<Kiaro::Common::F32>();
                    position.Y = in.read<Kiaro::Common::F32>();
                    position.X = in.read<Kiaro::Common::F32>();

                    mSceneNode->setPosition(position);
                }
            }

            void CTerrain::instantiate(void)
            {
                Kiaro::Engine::FileReadObject fileHandle(mTerrainFile);

                irr::IrrlichtDevice *irrlichtDevice = Kiaro::Engine::SEngineInstance::getPointer()->getIrrlichtDevice();
                irr::scene::ITerrainSceneNode *terrain = irrlichtDevice->getSceneManager()->addTerrainSceneNode(&fileHandle);

                if (terrain)
                {
                    terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);

                    mSceneNode = terrain;
                }
                else
                    std::cerr << "CTerrain: Failed to instantiate using '" << mTerrainFile << "'" << std::endl;
            }

            void CTerrain::update(const Kiaro::Common::F32 &deltaTimeSeconds)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
