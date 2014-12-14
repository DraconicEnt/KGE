/**
 *  @file Sky.cpp
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

#include <game/entities/Sky.hpp>
#include <game/entities/Types.hpp>
#include <game/ServerSingleton.hpp>

#include <support/BitStream.hpp>

#include <engine/FileReadObject.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            Sky::Sky() : Kiaro::Game::Entities::EntityBase(Kiaro::Game::Entities::ENTITY_SKY, Kiaro::Game::Entities::ENTITYHINT_NONTHINKING), mColor(0, 0, 0, 0)
            {
                instantiate();

                addNetworkedProperty("color", mColor);
                finalizeNetworkedProperties();
            }

            Sky::Sky(Kiaro::Support::BitStream &in) : Kiaro::Game::Entities::EntityBase(Kiaro::Game::Entities::ENTITY_SKY), mColor(0, 0, 0, 0)
            {
                unpackInitialization(in);
            }

            Sky::~Sky(void)
            {

            }

            void Sky::packUpdate(Kiaro::Support::BitStream &out)
            {

            }

            void Sky::unpackUpdate(Kiaro::Support::BitStream &in)
            {

            }

            void Sky::packInitialization(Kiaro::Support::BitStream &out)
            {

            }

            void Sky::unpackInitialization(Kiaro::Support::BitStream &in)
            {

            }

            void Sky::packData(Kiaro::Support::BitStream &out)
            {

            }

            void Sky::unpackData(Kiaro::Support::BitStream &in)
            {

            }

            void Sky::instantiate(void)
            {
                Kiaro::Game::Entities::EntityBase::instantiate();
            }

            void Sky::update(const Kiaro::Common::F32 &deltaTimeSeconds)
            {

            }

            const Kiaro::Common::ColorRGBA &Sky::getColor(void)
            {
                return mColor;
            }
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
