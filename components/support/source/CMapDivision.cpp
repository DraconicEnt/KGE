/**
 *  @file MapDivision.h
 *  @brief Header file defining the Kiaro::Support::MapDivision class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include <support/FTime.hpp>
#include <support/CMapDivision.hpp>

namespace Kiaro
{
    namespace Support
    {
        // MapDivisionSquare Code Begin
        CMapDivisionSquare::CMapDivisionSquare(void)
        {
            //std::cout << "TEST" << std::endl;
        }

        CMapDivision::CMapDivision(Common::U32 power, Common::U32 divisions) : mDivisions(divisions), mResolution(pow(2, power))
        {
            Support::FTime::timer timerHandle = Support::FTime::startTimer();
            //  Core::Logging::write(Core::Logging::MESSAGE_INFO, "CMapDivision: Initializing with a %ux%u map size ... ", mResolution, mResolution);
            Common::U32 currentResolution = mResolution;

            while (currentResolution > 1)
            {
                Common::U32 currentLODDivision = mResolution / currentResolution;
                std::vector<std::vector<CMapDivisionSquare>> currentLOD;

                for (Common::U32 iterationX = 0; iterationX < currentLODDivision; iterationX++)
                {
                    std::vector<CMapDivisionSquare> currentYAxis;
                    currentLOD.push_back(currentYAxis);

                    for (Common::U32 iterationY = 0; iterationY < currentLODDivision; iterationY++)
                        currentYAxis.push_back(CMapDivisionSquare());
                }

                // Push the last element to the vector again to prevent out of bounds access under certain improbable circumstances
                currentLOD.push_back(currentLOD[currentLOD.size() - 1]);
                mLODs.push_back(currentLOD);
                currentResolution >>= 1;
            }//Core::Logging::write(Core::Logging::MESSAGE_INFO, "CMapDivision: Built grid in %f seconds.", Support::FTime::stopTimer(timerHandle));
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
