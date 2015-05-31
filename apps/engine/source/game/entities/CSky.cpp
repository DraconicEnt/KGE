/**
 *  @file CSky.cpp
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

#include <game/entities/CSky.hpp>
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
            CSky::CSky() : Game::Entities::IEntity(Game::Entities::ENTITY_SKY, Game::Entities::ENTITYHINT_NONTHINKING), mColor(0, 0, 0, 0)
            {
                instantiate();

                addNetworkedProperty("color", mColor);
            }

            CSky::CSky(Support::CBitStream &in) : Game::Entities::IEntity(Game::Entities::ENTITY_SKY), mColor(0, 0, 0, 0)
            {
                unpackInitialization(in);
            }

            CSky::~CSky(void)
            {

            }

            void CSky::packUpdate(Support::CBitStream &out)
            {

            }

            void CSky::unpackUpdate(Support::CBitStream &in)
            {

            }

            void CSky::packInitialization(Support::CBitStream &out)
            {

            }

            void CSky::unpackInitialization(Support::CBitStream &in)
            {

            }

            void CSky::writeTo(Support::CBitStream &out) const
            {

            }

            void CSky::extractFrom(Support::CBitStream &in)
            {

            }

            void CSky::instantiate(void)
            {
                Game::Entities::IEntity::instantiate();
            }

            void CSky::update(const Common::F32 &deltaTimeSeconds)
            {

            }

            const Common::ColorRGBA &CSky::getColor(void)
            {
                return mColor;
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
