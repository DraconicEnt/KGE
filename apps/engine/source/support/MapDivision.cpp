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

#include <support/Time.hpp>
#include <support/MapDivision.hpp>

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
        static MapDivision *MapDivisionInstance = NULL;

        MapDivision *MapDivision::Get(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions)
        {
            if (!MapDivisionInstance)
                MapDivisionInstance = new Kiaro::Support::MapDivision(power, divisions);

            return MapDivisionInstance;
        }

        void MapDivision::Destroy(void)
        {
            if (MapDivisionInstance)
            {
                delete MapDivisionInstance;
                MapDivisionInstance = NULL;
            }
        }

        MapDivision::MapDivision(Kiaro::Common::U32 power, Kiaro::Common::U32 divisions) : mDivisions(divisions), mResolution(pow(2, power))
        {
            Kiaro::Support::Time::timer timerHandle = Kiaro::Support::Time::startTimer();
            std::cout << "MapDivision: Initializing with a " << mResolution << "x" << mResolution << " map size ..." << std::endl;

            Kiaro::Common::U32 currentResolution = mResolution;
            while (currentResolution > 1)
            {
                Kiaro::Common::U32 currentLODDivision = mResolution / currentResolution;

                std::vector<std::vector<MapDivisionSquare>> currentLOD;
                for (Kiaro::Common::U32 iterationX = 0; iterationX < currentLODDivision; iterationX++)
                {
                    std::vector<MapDivisionSquare> currentYAxis;
                    currentLOD.push_back(currentYAxis);

                    for (Kiaro::Common::U32 iterationY = 0; iterationY < currentLODDivision; iterationY++)
                        currentYAxis.push_back(MapDivisionSquare());
                }

                // Push the last element to the vector again to prevent out of bounds access under certain improbable circumstances
                currentLOD.push_back(currentLOD[currentLOD.size() - 1]);
                mLODs.push_back(currentLOD);

                currentResolution >>= 1;
            }

            std::cout << "MapDivision: Built grid in " << Kiaro::Support::Time::stopTimer(timerHandle) << " seconds " << std::endl;
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
