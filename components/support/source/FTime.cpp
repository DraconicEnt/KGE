/**
 *  @file FTime.cpp
 *  @brief Source file implementing timing operations.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <numeric>
#include <algorithm>

#include <support/FTime.hpp>
#include <support/Stack.hpp>
#include <support/String.hpp>

#include <support/types.hpp>
#include <support/platform/time.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            static Support::Stack<Common::U64> sTimerStack;
            static Common::U32 sCurrentSimTime = 0;

            Support::FTime::timer startTimer(void)
            {
                sTimerStack.push(Platform::Time::getCurrentTimeMicroseconds());
                return sTimerStack.size();
            }

            Common::F32 stopTimer(const Support::FTime::timer& timerIdentifier)
            {
                if (sTimerStack.size() == 0)
                    throw std::runtime_error("FTime: No timers to stop!");
                else if (sTimerStack.size() >= std::numeric_limits<Common::U8>::max())
                    throw std::runtime_error("FTime: Too many timers on stack!");
                else if (timerIdentifier != sTimerStack.size())
                {
                    Support::String errorString = "FTime: Mismatched timer identifier in stopTimer! Current: ";
                    errorString += sTimerStack.size();
                    errorString += " Given: ";
                    errorString += timerIdentifier;
                    throw std::runtime_error(errorString);
                }

                const Common::U64 lastTimeMicroseconds = sTimerStack.top();
                const Common::U64 currentTimeMicroseconds = Platform::Time::getCurrentTimeMicroseconds();
                // NOTE (Robert MacGregor#1): Prevents the conversion calculation below from potentially being unrepresentable
                Common::U64 deltaTimeMicroseconds = currentTimeMicroseconds - lastTimeMicroseconds;

                // Only add deltas if we're the upper most timer
                if(timerIdentifier == 1)
                    sCurrentSimTime += deltaTimeMicroseconds;

                deltaTimeMicroseconds = std::max(static_cast<Common::U64>(100), deltaTimeMicroseconds);
                Common::F32 result = (Common::F32)(deltaTimeMicroseconds) / 1000000.f;
                sTimerStack.pop();
                return result;
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
