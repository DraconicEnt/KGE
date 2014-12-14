/**
 *  @file Sky.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_

#include "engine/Common.hpp"

#include <game/entities/EntityBase.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        class BitStream;
    }

    namespace Game
    {
        namespace Entities
        {
            class Sky : public Kiaro::Game::Entities::EntityBase
            {
                // Public Methods
                public:
                    Sky();
                    Sky(Kiaro::Support::BitStream &in);

                    ~Sky(void);

                    virtual void packUpdate(Kiaro::Support::BitStream &out);
                    virtual void unpackUpdate(Kiaro::Support::BitStream &in);
                    virtual void packInitialization(Kiaro::Support::BitStream &out);
                    virtual void unpackInitialization(Kiaro::Support::BitStream &in);
                    virtual void instantiate(void);
                    virtual void update(const Kiaro::Common::F32 &deltaTimeSeconds);

                // Private Members
                private:
                    Kiaro::Common::ColorRGBA mColor;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_
