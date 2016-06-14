/**
 *  @file SProfiler.cpp
 *  @brief Source file containing coding for profiler tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include <support/SProfiler.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            TEST(SProfiler, NoBegin)
            {
                EXPECT_THROW(PROFILER_END(TestScope), std::runtime_error);
                Support::SProfiler::destroy();
            }

            TEST(SProfiler, BrokenBegin)
            {
                EXPECT_NO_THROW(PROFILER_BEGIN(TestScope));
                EXPECT_THROW(PROFILER_BEGIN(TestScope), std::runtime_error);
                Support::SProfiler::destroy();
            }

            TEST(SProfiler, Measure)
            {
                EXPECT_NO_THROW(PROFILER_BEGIN(TestScope));
                std::this_thread::sleep_for(std::chrono::milliseconds(32));
                EXPECT_NO_THROW(PROFILER_END(TestScope));

                // At this point, the first measure for 'TestScope' should measure ~32ms
                Common::F32 measuredTime = Support::SProfiler::getPointer()->getSample("TestScope", 0);
                EXPECT_TRUE(measuredTime <= 0.034f && measuredTime >= 0.031f);

                // What about multiple measures?
                EXPECT_NO_THROW(PROFILER_BEGIN(AnotherScope));
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                EXPECT_NO_THROW(PROFILER_END(AnotherScope));

                measuredTime = Support::SProfiler::getPointer()->getSample("AnotherScope", 0);
                EXPECT_TRUE(measuredTime <= 0.018f && measuredTime >= 0.014f);

                // Is the original measure still right?
                measuredTime = Support::SProfiler::getPointer()->getSample("TestScope", 0);
                EXPECT_TRUE(measuredTime <= 0.034f && measuredTime >= 0.031f);
                Support::SProfiler::destroy();
            }

            TEST(SProfiler, InvalidSample)
            {
                EXPECT_NO_THROW(PROFILER_BEGIN(TestScope));
                EXPECT_NO_THROW(PROFILER_END(TestScope));

                EXPECT_THROW(Support::SProfiler::getPointer()->getSample("TestScope", 900), std::out_of_range);
                EXPECT_THROW(Support::SProfiler::getPointer()->getSample("RandomScope", 0), std::out_of_range);
                EXPECT_THROW(Support::SProfiler::getPointer()->getSample("RandomScope", 900), std::out_of_range);

                Support::SProfiler::destroy();
            }

            TEST(SProfiler, Average)
            {
                Support::SProfiler* profiler = Support::SProfiler::getPointer(6);

                for (size_t iteration = 0; iteration < 7; iteration++)
                {
                    EXPECT_NO_THROW(PROFILER_BEGIN(TestScope));
                    std::this_thread::sleep_for(std::chrono::milliseconds(16));
                    EXPECT_NO_THROW(PROFILER_END(TestScope));
                    profiler->update();
                }

                // Ensure we have only one name entry
                EXPECT_EQ(1, profiler->getSampleNames().size());

                // The average should be ~16ms
                Common::F32 average = profiler->getAverage("TestScope");
                EXPECT_TRUE(average <= 0.018f && average >= 0.014f);

                // We should also report an error if we don't even know what the sample scope is
                EXPECT_THROW(profiler->getAverage("RandomScope"), std::out_of_range);
                Support::SProfiler::destroy();
            }

            TEST(SProfiler, AverageReport)
            {
                Support::SProfiler* profiler = Support::SProfiler::getPointer(6);

                for (size_t iteration = 0; iteration < 7; iteration++)
                {
                    EXPECT_NO_THROW(PROFILER_BEGIN(OneScope));
                    EXPECT_NO_THROW(PROFILER_BEGIN(TwoScope));
                    EXPECT_NO_THROW(PROFILER_BEGIN(ThreeScope));
                    EXPECT_NO_THROW(PROFILER_BEGIN(FourScope));

                    std::this_thread::sleep_for(std::chrono::milliseconds(16));

                    EXPECT_NO_THROW(PROFILER_END(FourScope));
                    EXPECT_NO_THROW(PROFILER_END(ThreeScope));
                    EXPECT_NO_THROW(PROFILER_END(TwoScope));
                    EXPECT_NO_THROW(PROFILER_END(OneScope));
                    profiler->update();
                }

                // Obtain a report
                Support::Set<std::pair<Support::String, Common::F32>> report = profiler->getSampleAverages();
                EXPECT_EQ(4, report.size());

                // All averages should be ~16ms
                for (auto it = report.begin(); it != report.end(); it++)
                {
                    const std::pair<Support::String, Common::F32>& result = *it;
                    EXPECT_TRUE(result.second <= 0.018f && result.second >= 0.014f);
                }

                Support::SProfiler::destroy();
            }
        }
    } // End Namespace Support
} // End namespace Kiaro


