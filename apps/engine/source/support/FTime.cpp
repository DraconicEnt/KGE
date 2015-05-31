/**
 *  @file FTime.cpp
 *  @brief Source file implementing timing operations.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <algorithm>

#include <sys/time.h>

#include <support/FTime.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            static std::vector<Common::U64> sTimerStack;
            static Common::U32 sCurrentSimTime = 0;

            Support::FTime::timer startTimer(void)
            {
                sTimerStack.push_back(Support::FTime::getCurrentTimeMicroseconds());
                return sTimerStack.size();
            }

            Common::F32 stopTimer(const Support::FTime::timer &timerIdentifier)
            {
                if (sTimerStack.size() == 0)
                    throw std::logic_error("FTime: No timers to stop!");
                else if (timerIdentifier != sTimerStack.size())
                    throw std::logic_error("FTime: Mismatched timer identifier!");

                const Common::U64 lastTimeMicroseconds = sTimerStack.back();
                const Common::U64 currentTimeMicroseconds = Support::FTime::getCurrentTimeMicroseconds();

                // NOTE (Robert MacGregor#1): Prevents the conversion calculation below from potentially being unrepresentable
                Common::U64 deltaTimeMicroseconds = currentTimeMicroseconds - lastTimeMicroseconds;

                // Only add deltas if we're the upper most timer
                if(timerIdentifier == 1)
                    sCurrentSimTime += deltaTimeMicroseconds;

                deltaTimeMicroseconds = std::max(static_cast<Common::U64>(100), deltaTimeMicroseconds);

                Common::F32 result = (Common::F32)(deltaTimeMicroseconds) / 1000000.f;
                sTimerStack.pop_back();

                return result;
            }

            Common::U64 getCurrentTimeMicroseconds(void)
            {
                timeval currentTime;
                gettimeofday(&currentTime, NULL);

                return currentTime.tv_usec + (1000000ULL * currentTime.tv_sec);
            }

            Common::U64 getTimerResolutionMicroseconds(void)
            {
                //timespec tp;
                //clock_getres(CLOCK_MONOTONIC, &tp);

                //return tp.tv_nsec;
                return 0;
            }

            Common::U64 getSimTimeMicroseconds(void)
            {
                return sCurrentSimTime;
            }

            Common::U64 getSimTimeMilliseconds(void)
            {
                return sCurrentSimTime / 1000ULL;
            }
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
