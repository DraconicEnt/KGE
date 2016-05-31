/**
 *  @file CTerrain.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Robert MacGregor
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2015 Draconic Entity
 */

#include <support/Console.hpp>
#include <game/entities/CTerrain.hpp>
#include <game/entities/types.hpp>

#include <net/IServer.hpp>

#include <support/CBitStream.hpp>

#include <filesystem/CFileReader.hpp>

#include <video/SRenderer.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            CTerrain::CTerrain(const Support::String& terrainFile) : IRigidObject(ENTITY_TERRAIN, FLAG_STATIC),
                mTerrainFile(terrainFile), mSceneNode(nullptr)
            {
            }

            CTerrain::CTerrain(Support::CBitStream& in) : IRigidObject(ENTITY_TERRAIN)
            {
                this->unpack(in);
            }

            CTerrain::~CTerrain(void)
            {
            }

            void CTerrain::packEverything(Support::CBitStream& out) const
            {
                assert(mSceneNode);
                IEntity::packEverything(out);
                const Common::Vector3DF& position = mSceneNode->getPosition();
                out.writeString(mTerrainFile);
                out << position.X << position.Y << position.Z;
            }

            void CTerrain::unpack(Support::CBitStream& in)
            {
                mTerrainFile = in.popString();
                Common::Vector3DF position;
                in >> position.X >> position.Y >> position.Z;
                this->registerEntity();

                if (mSceneNode)
                    mSceneNode->setPosition(position);
            }

            void CTerrain::registerEntity(void)
            {
                CONSOLE_INFOF("Building terrain with file '%s' ...", mTerrainFile.data());
                FileSystem::CFileReader fileHandle(mTerrainFile);
                irr::IrrlichtDevice* irrlichtDevice = Video::SRenderer::getPointer()->getIrrlichtDevice();
                irr::scene::ITerrainSceneNode* terrain = irrlichtDevice->getSceneManager()->addTerrainSceneNode(&fileHandle);

                if (terrain)
                {
                    terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
                    terrain->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
                    mSceneNode = terrain;
                    IEntity::registerEntity();
                }
                else
                    CONSOLE_ERROR("Failed to instantiate using terrain file '%s'", mTerrainFile.data());
            }

            void CTerrain::setPosition(const Common::Vector3DF& position)
            {
                mSceneNode->setPosition(position);
            }

            void CTerrain::update(const Common::F32 deltaTimeSeconds)
            {
            }

            size_t CTerrain::getRequiredMemory(void) const
            {
                return sizeof(Common::F32) * 3;
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
