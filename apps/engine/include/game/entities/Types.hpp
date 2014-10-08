/**
 *  @file Types.hpp
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
                ENTITY_RIGIDPROP = (1 << 0),
                ENTITY_TERRAIN = (2 << 0),
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_ENTITIES_TYPES_HPP_
