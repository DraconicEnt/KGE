/**
 *  @file SThreadSystem.cpp
 *  @brief Source file containing coding for the SThreadSystem tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/SSettingsRegistry.hpp>
#include <support/tasking/SThreadSystem.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Tasking
        {
            static bool FirstGameStateValue = false;
            static void setFirstGameStateValue(bool value)
            {
                CONSOLE_INFOF("Set First Game State: %s", value ? "True" : "False");
                FirstGameStateValue = value;
            }

            static float SecondGameStateValue = 0.0f;
            static void setSecondGameStateValue(float value)
            {
                CONSOLE_INFOF("Set Second Game State: %f", value);
                SecondGameStateValue = value;
            }

            static char* ThirdGameStateValue = nullptr;
            static void setThirdGameStateValue(const char* value)
            {
                CONSOLE_INFOF("Set Third Game State: %s", value);
                ThirdGameStateValue = const_cast<char*>(value);
            }

            class ActionOne : public SThreadSystem::IThreadedTask
            {
                public:
                    ActionOne(void)
                    {
                        mProcessingIndex = 0;
                        mResources.insert(1);
                    }

                    void initialize(void)
                    {

                    }

                    void deinitialize(void)
                    {

                    }

                    bool tick(const Common::F32 deltaTimeSeconds)
                    {
                        mTransaction.push(new EasyDelegate::DeferredStaticCaller<void, bool>(setFirstGameStateValue, true));
                        return true;
                    }
            };

            class ActionTwoPhaseOne : public SThreadSystem::IThreadedTask
            {
                public:
                    ActionTwoPhaseOne(void)
                    {
                        mProcessingIndex = 0;
                        mResources.insert(1);
                    }

                    void initialize(void)
                    {

                    }

                    void deinitialize(void)
                    {

                    }

                    bool tick(const Common::F32 deltaTimeSeconds)
                    {
                        mTransaction.push(new EasyDelegate::DeferredStaticCaller<void, float>(setSecondGameStateValue, 3.14f));
                        return true;
                    }
            };

            class ActionTwoPhaseTwo : public SThreadSystem::IThreadedTask
            {
                public:
                    ActionTwoPhaseTwo(void)
                    {
                        mProcessingIndex = 1;
                    }

                    void initialize(void)
                    {

                    }

                    void deinitialize(void)
                    {

                    }

                    bool tick(const Common::F32 deltaTimeSeconds)
                    {
                        mTransaction.push(new EasyDelegate::DeferredStaticCaller<void, float>(setSecondGameStateValue, 3117.0f));
                        return true;
                    }
            };

            class ActionThree : public SThreadSystem::IThreadedTask
            {
                public:
                    ActionThree(void)
                    {
                        mProcessingIndex = 1;
                    }

                    void initialize(void)
                    {

                    }

                    void deinitialize(void)
                    {

                    }

                    bool tick(const Common::F32 deltaTimeSeconds)
                    {
                        mTransaction.push(new EasyDelegate::DeferredStaticCaller<void, const char*>(setThirdGameStateValue, "Test String"));
                        return true;
                    }
            };

            TEST(SThreadSystem, Transactions)
            {
                // We initialize the runtime thread count to something predictable
                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getInstance();
                settings->setValue<Common::U8>("System::RuntimeThreadCount", 25);

                // Initialize the thread system and two phases
                SThreadSystem* threading = SThreadSystem::getInstance();
                threading->addTask(new ActionOne());
                threading->addTask(new ActionTwoPhaseOne());
                threading->addTask(new ActionTwoPhaseTwo());
                threading->addTask(new ActionThree());

                for (Common::U32 iteration = 0; iteration < 8; ++iteration)
                {
                    // We wait a little bit for our actions to process
                    EXPECT_FALSE(FirstGameStateValue);
                    EXPECT_EQ(0.0f, SecondGameStateValue);
                    EXPECT_EQ(nullptr, ThirdGameStateValue);

                    // Wait for the first cycle
                    threading->update();
                    std::this_thread::sleep_for(std::chrono::milliseconds(32));
                    threading->update();

                    EXPECT_TRUE(FirstGameStateValue);
                    EXPECT_EQ(3.14f, SecondGameStateValue);
                    EXPECT_EQ(nullptr, ThirdGameStateValue);

                    // Restart the values for the next cycle
                    FirstGameStateValue = false;
                    SecondGameStateValue = 0.0f;
                    ThirdGameStateValue = nullptr;

                    // Run the next cycle
                    threading->update();
                    std::this_thread::sleep_for(std::chrono::milliseconds(32));
                    threading->update();

                    EXPECT_FALSE(FirstGameStateValue);
                    EXPECT_EQ(3117.0f, SecondGameStateValue);
                    EXPECT_NE(nullptr, ThirdGameStateValue);

                    if (ThirdGameStateValue)
                        EXPECT_EQ(0, strcmp(ThirdGameStateValue, "Test String"));

                    // Restart the values for the next iteration
                    FirstGameStateValue = false;
                    SecondGameStateValue = 0.0f;
                    ThirdGameStateValue = nullptr;
                }

                Support::SSettingsRegistry::destroy();
                SThreadSystem::destroy();
            }
        }
    } // End Namespace Support
} // End namespace Kiaro
