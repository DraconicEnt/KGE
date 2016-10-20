/**
 *  @file CPlayer.cpp
 *  @brief Source file implementing the CPlayer methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entity
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/entities/CPlayer.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                CPlayer::CPlayer(void) : IRigidObject(ENTITY_TERRAIN, FLAG_STATIC)
                {

                }

                CPlayer::CPlayer(Support::CBitStream& in) : IRigidObject(ENTITY_TERRAIN, FLAG_STATIC)
                {
                    this->registerNetworkedProperties();
                    this->unpack(in);
                }

                void CPlayer::packEverything(Support::CBitStream& out) const
                {
                    this->packBaseData<CPlayer>(out);
                    IEntity::packEverything(out);
                }

                void CPlayer::unpack(Support::CBitStream& in)
                {
                    IEntity::unpack(in);

                    this->registerEntity();

                 //   if (mSceneNode)
                     //   mSceneNode->setPosition(mPosition);
                }

                void CPlayer::update(const Common::F32 deltaTimeSeconds)
                {

                }

                void CPlayer::registerEntity(void)
                {

                }

                size_t CPlayer::getRequiredMemory(void) const
                {
                    return sizeof(Common::F32) * 3;
                }
            }
        } // End Namespace Game
    } // End NameSpace Engine
} // End Namespace Kiaro
