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

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            CTerrain::CTerrain(const Support::String& terrainFile) : IRigidObject(ENTITY_TERRAIN, NO_THINKING | NO_SCOPING),
            mTerrainFile(terrainFile), mSceneNode(nullptr)
            {
                instantiate();
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
                const Common::Vector3DF& position = mSceneNode->getPosition();

                out << position.X << position.Y << position.Z;
                out.write(mTerrainFile);

                IEntity::packEverything(out);
            }

            void CTerrain::unpack(Support::CBitStream& in)
            {
                mTerrainFile = in.top<const Common::C8>();
                in.pop<const Common::C8*>();

                instantiate();

                Common::Vector3DF position;
                in >> position;

                if (mSceneNode)
                    mSceneNode->setPosition(position);
            }

            void CTerrain::instantiate(void)
            {
                Support::Console::writef(Support::Console::MESSAGE_INFO, "CTerrain: Building with file '%s' ...", mTerrainFile.data());
                FileSystem::FileReadObject fileHandle(mTerrainFile);

             //   irr::IrrlichtDevice* irrlichtDevice = Core::SEngineInstance::getPointer()->getIrrlichtDevice();
              //  irr::scene::ITerrainSceneNode* terrain = irrlichtDevice->getSceneManager()->addTerrainSceneNode(&fileHandle);

               // if (terrain)
               // {
                 //  terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
                  //  terrain->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);

                   // mSceneNode = terrain;
               // }
               // else
                //    Support::Console::write(Support::Console::MESSAGE_ERROR, "CTerrain: Failed to instantiate using '%s'", mTerrainFile.data());
            }

            void CTerrain::setPosition(const Common::Vector3DF& position)
            {
                mSceneNode->setPosition(position);
            }

            void CTerrain::update(const Common::F32& deltaTimeSeconds)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
