/**
 *  @file Time.cpp
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

#include <support/Time.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Time
        {
            static std::vector<Kiaro::Common::U64> sTimerStack;
            static Kiaro::Common::U32 sCurrentSimTime = 0;

            Kiaro::Support::Time::timer startTimer(void)
            {
                sTimerStack.push_back(Kiaro::Support::Time::getCurrentTimeMicroseconds());
                return sTimerStack.size();
            }

            Kiaro::Common::F32 stopTimer(const Kiaro::Support::Time::timer &timerIdentifier)
            {
                if (sTimerStack.size() == 0)
                    throw std::logic_error("Time: No timers to stop!");
                else if (timerIdentifier != sTimerStack.size())
                    throw std::logic_error("Time: Mismatched timer identifier!");

                const Kiaro::Common::U64 lastTimeMicroseconds = sTimerStack.back();
                const Kiaro::Common::U64 currentTimeMicroseconds = Kiaro::Support::Time::getCurrentTimeMicroseconds();

                // NOTE (Robert MacGregor#1): Prevents the conversion calculation below from potentially being unrepresentable
                Kiaro::Common::U64 deltaTimeMicroseconds = currentTimeMicroseconds - lastTimeMicroseconds;

                // Only add deltas if we're the upper most timer
                if(timerIdentifier == 1)
                    sCurrentSimTime += deltaTimeMicroseconds;

                deltaTimeMicroseconds = std::max(static_cast<Kiaro::Common::U64>(100), deltaTimeMicroseconds);

                Kiaro::Common::F32 result = (Kiaro::Common::F32)(deltaTimeMicroseconds) / 1000000.f;
                sTimerStack.pop_back();

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
                //timespec tp;
                //clock_getres(CLOCK_MONOTONIC, &tp);

                //return tp.tv_nsec;
                return 0;
            }

            Kiaro::Common::U64 getSimTimeMicroseconds(void)
            {
                return sCurrentSimTime;
            }

            Kiaro::Common::U64 getSimTimeMilliseconds(void)
            {
                return sCurrentSimTime / 1000ULL;
            }
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
