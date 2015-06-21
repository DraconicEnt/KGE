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

#include <net/SServer.hpp>

#include <support/CBitStream.hpp>

#include <filesystem/CFileReader.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            CTerrain::CTerrain(const Support::String& terrainFile) : IRigidObject(ENTITY_TERRAIN),
            mTerrainFile(terrainFile)
            {
                instantiate();
            }

            CTerrain::CTerrain(Support::CBitStream& in) : IRigidObject(ENTITY_TERRAIN)
            {
                unpackInitialization(in);
            }

            CTerrain::~CTerrain(void)
            {

            }

            void CTerrain::packUpdate(Support::CBitStream& out)
            {


            }

            void CTerrain::unpackUpdate(Support::CBitStream& in)
            {

            }

            void CTerrain::packInitialization(Support::CBitStream& out)
            {
                const Common::Vector3DF& position = mSceneNode->getPosition();

                out << position.X << position.Y << position.Z << mTerrainFile;
            }

            void CTerrain::unpackInitialization(Support::CBitStream& in)
            {
                in >> mTerrainFile;

                instantiate();

                if (mSceneNode)
                {
                    Common::Vector3DF position;
                    in >> position;

                   // position.Z = in.read<Common::F32>();
                    //position.Y = in.read<Common::F32>();
                   // position.X = in.read<Common::F32>();

                    mSceneNode->setPosition(position);
                }
            }

            void CTerrain::instantiate(void)
            {
                FileSystem::FileReadObject fileHandle(mTerrainFile);

                irr::IrrlichtDevice* irrlichtDevice = Core::SEngineInstance::getPointer()->getIrrlichtDevice();
                irr::scene::ITerrainSceneNode* terrain = irrlichtDevice->getSceneManager()->addTerrainSceneNode(&fileHandle);

                if (terrain)
                {
                    terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);

                    mSceneNode = terrain;
                }
                else
                    std::cerr << "CTerrain: Failed to instantiate using '" << mTerrainFile << "'" << std::endl;
            }

            void CTerrain::update(const Common::F32& deltaTimeSeconds)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
