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

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include <support/MapDivision.h>

namespace Kiaro
{
    namespace Support
    {
        // MapDivisionSquare Code Begin
        MapDivisionSquare::MapDivisionSquare(void)
        {
            //std::cout << "TEST" << std::endl;
        }


        // MapDivision Code Begin
        MapDivision::MapDivision(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions) : Divisions(divisions), Resolution(pow(2, power))
        {
            Kiaro::Common::U32 current_resolution = this->Resolution;

            while (current_resolution > 1)
            {
                // Since both Resolution and current_resolution are powers of 2, this should count up in powers of 2 starting at 1
                Kiaro::Common::U32 current_lod_division = this->Resolution / current_resolution;

                std::cout << "LOD Div: " << current_lod_division << std::endl;
                std::cout << "Resolution: " << current_resolution << std::endl;

                // Build the 2D array to be +1 elements on both axiis that refer to the actual maximum to prevent out of index operations under improbable circumstances
                MapDivisionSquare** lod_contents = new MapDivisionSquare*[current_lod_division + 1];
                for (Kiaro::Common::U32 iterations = 0; iterations < current_lod_division; iterations++)
                    if (iterations == current_lod_division - 1)
                        lod_contents[iterations] = lod_contents[iterations - 1];
                    else
                        lod_contents[iterations] = new MapDivisionSquare[current_lod_division + 1];

                std::cout << "Appended ---" << std::endl;

                this->LODs.Append(lod_contents);
                current_resolution >>= 1;
            }
        }

        MapDivision::~MapDivision(void)
        {
            for (Kiaro::Common::U32 iterations = 0; iterations < this->LODs.Len(); iterations++)
            {
               // for (Kiaro::Common::U32 sub_iterations = 0; sub_iterations <)
               // delete[] this->LODs[iterations];
            }
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
