/**
 *  @file CMapDivision.hpp
 *  @brief Header file defining the CMapDivision class.
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
            // Public Methods
            public:
                //! Parameter-less constructor.
                CMapDivisionSquare(void);

                std::vector<CES::Entity*> mContents;
        };

        /**
         *  @brief The CMapDivison class is a general data structure intended for use in improving the speed of radius searches
         *  by arranging the map into a series of squares at different scales called "LODs".
         *  @details When an object's position is updated, its new square ownership is calculated for each LOD. This allows us to check
         *  specific grid sections in some specific distance around a point (if collapsing to square shaped regions is acceptable) to avoid
         *  checking objects not within these squares unnecessarily.
         */
        class CMapDivision
        {
            // Private Members
            private:
                //! 3D Array: LOD,X,Y
                std::vector<std::vector<std::vector<CMapDivisionSquare>>> mLODs;

            // Public Methods
            public:
                /**
                 *  @brief Constructor accepting a maximum power division and the number of divisions to make.
                 */
                CMapDivision(const Common::U32 power, const Common::U32 divisions = 999);

                //! The number of LOD's in play.
                const Common::U32 mDivisions;
                const size_t mResolution;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_CMAPDIVISION_HPP_
