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

#ifndef _INCLUDE_KIARO_SUPPORT_MAPDIVISION_HPP_
#define _INCLUDE_KIARO_SUPPORT_MAPDIVISION_HPP_

#include <engine/Common.hpp>

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

                std::vector<Kiaro::CES::Entity*> mContents;
        }; // End Class MapDivisionSquare

        class MapDivision
        {
            // Public Methods
            public:
                static MapDivision *Get(Kiaro::Common::U32 power = 12, Kiaro::Common::U32 divisions = 999);
                static void Destroy(void);

                const Kiaro::Common::U32 mDivisions;
                const size_t mResolution;

            // Private Members
            private:
                MapDivision(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions = 999);

                //! 3D Array: LOD,X,Y
                std::vector<std::vector<std::vector<MapDivisionSquare>>> mLODs;
        }; // End Class MapDivision
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_MAPDIVISION_HPP_
