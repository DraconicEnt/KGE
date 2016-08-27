/**
 *  @file CSky.cpp
 *  @brief Source file implementing the CSky class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/entities/CSky.hpp>
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
            CSky::CSky(void) : Game::Entities::IEntity(ENTITY_SKY, FLAG_STATIC) //, mColor(0, 0, 0, 0)
            {
                // addNetworkedProperty("color", mColor);
            }

            CSky::CSky(Support::CBitStream& in) : IEntity(ENTITY_SKY) //, mColor(0, 0, 0, 0)
            {
                unpackInitialization(in);
            }

            CSky::~CSky(void)
            {
            }

            void CSky::packUpdate(Support::CBitStream& out)
            {
            }

            void CSky::unpackUpdate(Support::CBitStream& in)
            {
            }

            void CSky::packInitialization(Support::CBitStream& out)
            {
            }

            void CSky::unpackInitialization(Support::CBitStream& in)
            {
            }

            void CSky::writeTo(Support::CBitStream& out) const
            {
            }

            void CSky::extractFrom(Support::CBitStream& in)
            {
            }

            void CSky::registerEntity(void)
            {
                IEntity::registerEntity();
            }

            void CSky::update(const Common::F32 deltaTimeSeconds)
            {
            }

            // const Common::ColorRGBA& CSky::getColor(void)
            // {
            //     return mColor;
            // }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
