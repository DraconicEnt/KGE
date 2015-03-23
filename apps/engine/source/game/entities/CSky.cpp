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
#include <network/SServer.hpp>

#include <support/CBitStream.hpp>

#include <engine/CFileReader.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            CSky::CSky() : Kiaro::Game::Entities::IEntity(Kiaro::Game::Entities::ENTITY_SKY, Kiaro::Game::Entities::ENTITYHINT_NONTHINKING), mColor(0, 0, 0, 0)
            {
                instantiate();

                addNetworkedProperty("color", mColor);
            }

            CSky::CSky(Kiaro::Support::CBitStream &in) : Kiaro::Game::Entities::IEntity(Kiaro::Game::Entities::ENTITY_SKY), mColor(0, 0, 0, 0)
            {
                unpackInitialization(in);
            }

            CSky::~CSky(void)
            {

            }

            void CSky::packUpdate(Kiaro::Support::CBitStream &out)
            {

            }

            void CSky::unpackUpdate(Kiaro::Support::CBitStream &in)
            {

            }

            void CSky::packInitialization(Kiaro::Support::CBitStream &out)
            {

            }

            void CSky::unpackInitialization(Kiaro::Support::CBitStream &in)
            {

            }

            void CSky::packData(Kiaro::Support::CBitStream &out)
            {

            }

            void CSky::unpackData(Kiaro::Support::CBitStream &in)
            {

            }

            void CSky::instantiate(void)
            {
                Kiaro::Game::Entities::IEntity::instantiate();
            }

            void CSky::update(const Kiaro::Common::F32 &deltaTimeSeconds)
            {

            }

            const Kiaro::Common::ColorRGBA &CSky::getColor(void)
            {
                return mColor;
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
