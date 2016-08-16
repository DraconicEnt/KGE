/**
 *  @file CMapDivision.hpp
 *  @brief Header file defining the Kiaro::Support::CMapDivision class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_
#define _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_

#include <support/types.hpp>
#include <support/common.hpp>

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

                std::vector<CES::Entity*> mContents;
        }; // End Class CMapDivisionSquare

        class CMapDivision
        {
                // Public Methods
            public:
                CMapDivision(Common::U32 power, Common::U32 divisions = 999);

                const Common::U32 mDivisions;
                const size_t mResolution;

                // Private Members
            private:


                //! 3D Array: LOD,X,Y
                std::vector<std::vector<std::vector<CMapDivisionSquare>>> mLODs;
        }; // End Class CMapDivision
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_
