/**
 *  @file CTerrain.cpp
 *  @brief Source file implementing the CTerrain class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/Console.hpp>
#include <game/entities/CTerrain.hpp>
#include <game/entities/types.hpp>

#include <net/IServer.hpp>

#include <support/types.hpp>
#include <support/CBitStream.hpp>

#include <filesystem/CFileReader.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                CTerrain::CTerrain(const Support::String& terrainFile, const Support::String& textureFile) : IRigidObject(ENTITY_TERRAIN, FLAG_STATIC),
                mTerrainFile(terrainFile), mTextureFile(textureFile) //, mSceneNode(nullptr)
                {
                    this->registerNetworkedProperties();
                }

                CTerrain::CTerrain(Support::CBitStream& in) : IRigidObject(ENTITY_TERRAIN)
                {
                    this->registerNetworkedProperties();
                    this->unpack(in);
                }

                CTerrain::~CTerrain(void)
                {
                }

                void CTerrain::packEverything(Support::CBitStream& out) const
                {
                   // assert(mSceneNode);

                    this->packBaseData<CTerrain>(out);
                    IEntity::packEverything(out);
                }

                void CTerrain::unpack(Support::CBitStream& in)
                {
                    IEntity::unpack(in);

                   // if (mSceneNode)
                   //     mSceneNode->setPosition(mPosition);
                }

                void CTerrain::registerEntity(void)
                {
                    CONSOLE_INFOF("Building terrain with file '%s' ...", mTerrainFile.data());

                    /*
                    FileSystem::CFileReader heightmapHandle(mTerrainFile);
                    irr::IrrlichtDevice* irrlichtDevice = Engine::Video::SRenderer::getInstance()->getIrrlichtDevice();
                    irr::scene::ITerrainSceneNode* terrain = irrlichtDevice->getSceneManager()->addTerrainSceneNode(&heightmapHandle);

                    if (terrain)
                    {
                        FileSystem::CFileReader textureHandle(mTextureFile);

                        irr::video::ITexture* texture = irrlichtDevice->getVideoDriver()->getTexture(&textureHandle);

                        if (texture)
                            terrain->setMaterialTexture(0, texture);

                        terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
                        terrain->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);

                        mSceneNode = terrain;
                        mSceneNodes.insert(mSceneNodes.end(), mSceneNode);

                        IEntity::registerEntity();
                    }
                    else
                        CONSOLE_ERROR("Failed to instantiate using terrain file '%s'", mTerrainFile.data());
                    */
                }

                void CTerrain::setPosition(const Support::Vector3DF& position)
                {
                    //mSceneNode->setPosition(position);
                }

                void CTerrain::update(const Common::F32 deltaTimeSeconds)
                {
                }

                size_t CTerrain::getRequiredMemory(void) const
                {
                    return sizeof(Common::F32) * 3;
                }

                void CTerrain::registerNetworkedProperties(void)
                {
                    IRigidObject::registerNetworkedProperties();
                    this->addNetworkedProperty("textureFile", mTextureFile);
                    this->addNetworkedProperty("terrainFile", mTerrainFile);
                }
            } // End Namespace Entities
        } // End Namespace Game
    }
} // End Namespace Kiaro
