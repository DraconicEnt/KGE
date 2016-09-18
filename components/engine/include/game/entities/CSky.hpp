/**
 *  @file CSky.hpp
 *  @brief Include file declaring the CSky class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_

#include "support/common.hpp"

#include <game/entities/IEntity.hpp>

#include <irrlicht.h>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    }

    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                /**
                 *  @brief Entity class representing a sky object.
                 */
                class CSky : public Game::Entities::IEntity
                {
                    // Public Methods
                    public:
                        CSky(void);
                        CSky(Support::CBitStream& in);

                        ~CSky(void);

                        virtual void packUpdate(Support::CBitStream& out);
                        virtual void unpackUpdate(Support::CBitStream& in);
                        virtual void writeTo(Support::CBitStream& out) const;
                        virtual void extractFrom(Support::CBitStream& in);
                        virtual void packInitialization(Support::CBitStream& out);
                        virtual void unpackInitialization(Support::CBitStream& in);
                        virtual void registerEntity(void);
                        virtual void update(const Common::F32 deltaTimeSeconds);

                        size_t getRequiredMemory(void)
                        {
                            return sizeof(Common::C8) * 4;
                        }
                };
            } // End Namespace Entities
        } // End Namespace Game
    }
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_SKY_HPP_
