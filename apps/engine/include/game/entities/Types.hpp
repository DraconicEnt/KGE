/**
 *  @file Types.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_GAME_ENTITIES_TYPES_HPP_
#define _INCLUDE_GAME_ENTITIES_TYPES_HPP_

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            enum ENTITY_TYPE
            {
                ENTITY_NULL = 0,
                ENTITY_RIGIDPROP = (1 << 1),
                ENTITY_TERRAIN = (2 << 1),
                ENTITY_SKY = (3 << 1),
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_ENTITIES_TYPES_HPP_
