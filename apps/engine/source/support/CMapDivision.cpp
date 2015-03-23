/**
 *  @file MapDivision.h
 *  @brief Header file defining the Kiaro::Support::MapDivision class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
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

        // MapDivision Code Begin
        static CMapDivision *MapDivisionInstance = NULL;

        CMapDivision *CMapDivision::Get(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions)
        {
            if (!MapDivisionInstance)
                MapDivisionInstance = new Kiaro::Support::CMapDivision(power, divisions);

            return MapDivisionInstance;
        }

        void CMapDivision::Destroy(void)
        {
            if (MapDivisionInstance)
            {
                delete MapDivisionInstance;
                MapDivisionInstance = NULL;
            }
        }

        CMapDivision::CMapDivision(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions) : mDivisions(divisions), mResolution(pow(2, power))
        {
            Kiaro::Support::FTime::timer timerHandle = Kiaro::Support::FTime::startTimer();
            std::cout << "CMapDivision: Initializing with a " << mResolution << "x" << mResolution << " map size ..." << std::endl;

            Kiaro::Common::U32 currentResolution = mResolution;
            while (currentResolution > 1)
            {
                Kiaro::Common::U32 currentLODDivision = mResolution / currentResolution;

                std::vector<std::vector<CMapDivisionSquare>> currentLOD;
                for (Kiaro::Common::U32 iterationX = 0; iterationX < currentLODDivision; iterationX++)
                {
                    std::vector<CMapDivisionSquare> currentYAxis;
                    currentLOD.push_back(currentYAxis);

                    for (Kiaro::Common::U32 iterationY = 0; iterationY < currentLODDivision; iterationY++)
                        currentYAxis.push_back(CMapDivisionSquare());
                }

                // Push the last element to the vector again to prevent out of bounds access under certain improbable circumstances
                currentLOD.push_back(currentLOD[currentLOD.size() - 1]);
                mLODs.push_back(currentLOD);

                currentResolution >>= 1;
            }

            std::cout << "CMapDivision: Built grid in " << Kiaro::Support::FTime::stopTimer(timerHandle) << " seconds " << std::endl;
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
