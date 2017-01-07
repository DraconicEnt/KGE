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

#include <chrono>
#include <numeric>
#include <algorithm>

#include <support/FTime.hpp>
#include <support/Stack.hpp>
#include <support/String.hpp>

#include <support/types.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            static Support::Stack<std::chrono::time_point<std::chrono::high_resolution_clock>> sTimerStack;
            static Common::U32 sCurrentSimTime = 0;

            Support::FTime::timer startTimer(void)
            {
                sTimerStack.push(std::chrono::high_resolution_clock::now());
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

                const auto lastTimePoint = sTimerStack.top();

                std::chrono::duration<Common::F32> deltaTimePoint = std::chrono::high_resolution_clock::now() - lastTimePoint;

                // Only add deltas if we're the upper most timer
                if(timerIdentifier == 1)
                    sCurrentSimTime += deltaTimePoint.count() * 1000000;

                sTimerStack.pop();
                return deltaTimePoint.count();
            }

            Common::U64 getSimTimeMilliseconds(void)
            {
                return sCurrentSimTime / 1000ULL;
            }

            void clearTimers(void)
            {
                sTimerStack = Support::Stack<std::chrono::time_point<std::chrono::high_resolution_clock>>();
            }
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
