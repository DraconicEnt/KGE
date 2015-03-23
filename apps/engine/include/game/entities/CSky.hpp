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

#include "engine/common.hpp"

#include <game/entities/IEntity.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    }

    namespace Game
    {
        namespace Entities
        {
            class CSky : public Kiaro::Game::Entities::IEntity
            {
                // Public Methods
                public:
                    CSky();
                    CSky(Kiaro::Support::CBitStream &in);

                    ~CSky(void);

                    virtual void packUpdate(Kiaro::Support::CBitStream &out);
                    virtual void unpackUpdate(Kiaro::Support::CBitStream &in);
                    virtual void packData(Kiaro::Support::CBitStream &out);
                    virtual void unpackData(Kiaro::Support::CBitStream &in);
                    virtual void packInitialization(Kiaro::Support::CBitStream &out);
                    virtual void unpackInitialization(Kiaro::Support::CBitStream &in);
                    virtual void instantiate(void);
                    virtual void update(const Kiaro::Common::F32 &deltaTimeSeconds);

                    const Kiaro::Common::ColorRGBA &getColor(void);

                // Private Members
                private:
                    Kiaro::Common::ColorRGBA mColor;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_
