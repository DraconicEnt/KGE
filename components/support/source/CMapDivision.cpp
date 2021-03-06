/**
 *  @file CMapDivision.cpp
 *  @brief Source file implementing the CMapDivision class methods.
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

#include <support/Console.hpp>
#include <support/FTime.hpp>
#include <support/CMapDivision.hpp>

namespace Kiaro
{
    namespace Support
    {
        CMapDivisionSquare::CMapDivisionSquare(void)
        {

        }

        CMapDivision::CMapDivision(const Common::U32 power, const Common::U32 divisions) : mDivisions(divisions), mResolution(pow(2, power))
        {
            Support::FTime::timer timerHandle = Support::FTime::startTimer();
            CONSOLE_INFOF("CMapDivision: Initializing with a %ux%u map size ... ", mResolution, mResolution);

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
                    {
                        currentYAxis.push_back(CMapDivisionSquare());
                    }
                }

                // Push the last element to the vector again to prevent out of bounds access under certain improbable circumstances
                currentLOD.push_back(currentLOD[currentLOD.size() - 1]);
                mLODs.push_back(currentLOD);
                currentResolution >>= 1;

                CONSOLE_INFOF("CMapDivision: Built grid in %f seconds.", Support::FTime::stopTimer(timerHandle));
            }
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
