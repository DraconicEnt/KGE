/**
 *  @file RigidProp.cpp
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

#include <game/entities/EntityBase.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            EntityBase::EntityBase(const Kiaro::Game::Entities::TypeMask &typeMask) : mTypeMask(typeMask), mNetID(0), mSceneNode(NULL)
            {
                addNetworkedProperty("position", mPosition);
            }

            EntityBase::~EntityBase(void)
            {
                if (mSceneNode)
                    mSceneNode->drop();
            }

            void EntityBase::setShapeFile(const std::string &filename)
            {
                irr::IrrlichtDevice *irrlichtDevice = Kiaro::Engine::CoreSingleton::getPointer()->getIrrlichtDevice();
                irr::scene::ISceneManager *sceneManager = irrlichtDevice->getSceneManager();

                // Create the Fileread object and give it to Irrlicht
                Kiaro::Engine::FileReadObject fileHandle(filename);

                irr::scene::IMesh *shapeFileMesh = sceneManager->getMesh(&fileHandle);

                if (!shapeFileMesh)
                {
                    std::cerr << "EntityBase: Cannot load shapefile '" << filename << "'" << std::endl;
                    return;
                }

                if (mSceneNode)
                    mSceneNode->drop();

                mSceneNode = sceneManager->addMeshSceneNode(shapeFileMesh);
            }

            Kiaro::Common::U32 EntityBase::getTypeMask(void) const { return mTypeMask; }

            Kiaro::Common::U32 EntityBase::getNetID(void) const { return mNetID; }

            void EntityBase::packUpdate(Kiaro::Support::BitStream &out)
            {

            }

            void EntityBase::unpackUpdate(Kiaro::Support::BitStream &in)
            {

            }

            void EntityBase::packInitialization(Kiaro::Support::BitStream &out)
            {

            }

            void EntityBase::unpackInitialization(Kiaro::Support::BitStream &in)
            {

            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
