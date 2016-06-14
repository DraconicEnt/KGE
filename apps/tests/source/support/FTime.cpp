/**
 *  @file FTime.cpp
 *  @brief Source file containing coding for the BitStream tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <chrono>
#include <thread>
#include <numeric>

#include <gtest/gtest.h>

#include <support/Stack.hpp>
#include <support/FTime.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            TEST(FTime, TimerAccuracy)
            {
                FTime::timer timer = FTime::startTimer();
                EXPECT_EQ(1, timer);

                // Sleep the thread
                std::this_thread::sleep_for(std::chrono::milliseconds(32));
                Common::F32 sleepTimeSeconds = FTime::stopTimer(timer);

                // We should be reasonably accurate about the timing
                EXPECT_TRUE(sleepTimeSeconds <= 0.034f && sleepTimeSeconds >= 0.031f);
            }

            TEST(FTime, TimerIdentifiers)
            {
                for (Common::U8 iteration = 1; iteration < std::numeric_limits<Common::U8>::max(); iteration++)
                    EXPECT_EQ(iteration, FTime::startTimer());

                // Then cleanup
                for (Common::U8 iteration = std::numeric_limits<Common::U8>::max() - 1; iteration > 0; iteration--)
                    EXPECT_NO_THROW(FTime::stopTimer(iteration));
            }
        }
    } // End Namespace Support
} // End namespace Kiaro

