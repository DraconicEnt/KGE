/**
 *  @file MapDivision.h
 *  @brief Header file defining the Kiaro::Support::MapDivision class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <EngineCommon.h>

#include <support/Array.hpp>

namespace Kiaro
{
    namespace CES
    {
        class Entity;
    } // End NameSpace CES

    namespace Support
    {
        class MapDivisionSquare
        {
            public:
            MapDivisionSquare(void);

            Kiaro::Support::Array<Kiaro::CES::Entity*> Contents;
        }; // End Class MapDivisionSquare

        class MapDivision
        {
            public:
            MapDivision(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions = 999);
            ~MapDivision(void);

            const Kiaro::Common::U32 Divisions;
            const Kiaro::Common::U32 Resolution;

            private:
            Kiaro::Support::Array<MapDivisionSquare**> LODs;
        }; // End Class MapDivision
    } // End NameSpace Support
} // End NameSpace Kiaro
