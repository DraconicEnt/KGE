/**
 *  @file CMapDivision.h
 *  @brief Header file defining the Kiaro::Support::MapDivision class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_
#define _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_

#include <engine/common.hpp>

namespace Kiaro
{
    namespace CES
    {
        class Entity;
    } // End NameSpace CES

    namespace Support
    {
        class CMapDivisionSquare
        {
            public:
                CMapDivisionSquare(void);

                std::vector<Kiaro::CES::Entity*> mContents;
        }; // End Class CMapDivisionSquare

        class CMapDivision
        {
            // Public Methods
            public:
                static CMapDivision *Get(Kiaro::Common::U32 power = 12, Kiaro::Common::U32 divisions = 999);
                static void Destroy(void);

                const Kiaro::Common::U32 mDivisions;
                const size_t mResolution;

            // Private Members
            private:
                CMapDivision(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions = 999);

                //! 3D Array: LOD,X,Y
                std::vector<std::vector<std::vector<CMapDivisionSquare>>> mLODs;
        }; // End Class CMapDivision
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_
