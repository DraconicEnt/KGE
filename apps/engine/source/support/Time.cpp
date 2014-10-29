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

#include <algorithm>

#include <sys/time.h>

#include <support/Time.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Time
        {
            static std::vector<Kiaro::Common::U64> timerStack;
            static Kiaro::Common::U32 currentSimTime = 0;

            Kiaro::Support::Time::timer startTimer(void)
            {
                timerStack.push_back(Kiaro::Support::Time::getCurrentTimeMicroseconds());
                return timerStack.size();
            }

            Kiaro::Common::F32 stopTimer(Kiaro::Support::Time::timer id)
            {
                if (timerStack.size() == 0)
                    throw std::logic_error("No timers to stop!");
                else if (id != timerStack.size())
                    throw std::logic_error("Mismatched timer identifier!");

                Kiaro::Common::U64 currentTimeMicroseconds = Kiaro::Support::Time::getCurrentTimeMicroseconds();
                Kiaro::Common::U64 lastTimeMicroseconds = timerStack.back();

                // NOTE (Robert MacGregor#1): Prevents the conversion calculation below from potentially being unrepresentable
                Kiaro::Common::U64 deltaTimeMicroseconds = currentTimeMicroseconds - lastTimeMicroseconds;

                // Only add deltas if we're the upper most timer
                if(id == 1)
                    currentSimTime += deltaTimeMicroseconds;

                deltaTimeMicroseconds = std::max(static_cast<Kiaro::Common::U64>(100), deltaTimeMicroseconds);

                Kiaro::Common::F32 result = (Kiaro::Common::F32)(deltaTimeMicroseconds) / 1000000.f;
                timerStack.pop_back();

                return result;
            }

            Kiaro::Common::U64 getCurrentTimeMicroseconds(void)
            {
                timeval currentTime;
                gettimeofday(&currentTime, NULL);

                return currentTime.tv_usec + (1000000ULL * currentTime.tv_sec);
            }

            Kiaro::Common::U64 getTimerResolutionMicroseconds(void)
            {
                timespec tp;
                clock_getres(CLOCK_MONOTONIC, &tp);

                return tp.tv_nsec;
            }

            Kiaro::Common::U64 getSimTimeMicroseconds(void)
            {
                return currentSimTime;
            }

            Kiaro::Common::U64 getSimTimeMilliseconds(void)
            {
                return currentSimTime / 1000ULL;
            }
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
