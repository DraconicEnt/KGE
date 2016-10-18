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
                FirstGameStateValue = value;
            }

            static float SecondGameStateValue = 0.0f;
            static void setSecondGameStateValue(float value)
            {
                SecondGameStateValue = value;
            }

            static char* ThirdGameStateValue = nullptr;
            static void setThirdGameStateValue(const char* value)
            {
                ThirdGameStateValue = const_cast<char*>(value);
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionOne(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, bool>(setFirstGameStateValue, true));

                return transaction;
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionTwo_PhaseOne(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, float>(setSecondGameStateValue, 3.14f));

                return transaction;
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionTwo_PhaseTwo(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, float>(setSecondGameStateValue, 3117.0f));

                return transaction;
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionThree(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, const char*>(setThirdGameStateValue, "Test String"));

                return transaction;
            }

            TEST(SThreadSystem, Transactions)
            {
                // We initialize the runtime thread count to something predictable
                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::instantiate();
                settings->setValue<Common::U8>("System::RuntimeThreadCount", 25);

                // Initialize the thread system and two phases
                SThreadSystem* threading = SThreadSystem::getPointer();
                Support::Vector<Support::Vector<SThreadSystem::ThreadAction>> phaseOne;
                Support::Vector<Support::Vector<SThreadSystem::ThreadAction>> phaseTwo;

                // Initialize two groups for phase 1
                Support::Vector<SThreadSystem::ThreadAction> phaseOneGroupOne;
                phaseOneGroupOne.insert(phaseOneGroupOne.end(), new EasyDelegate::StaticDelegate<Support::Queue<EasyDelegate::IDeferredCaller*>>(ActionTwo_PhaseOne));
                phaseOneGroupOne.insert(phaseOneGroupOne.end(), new EasyDelegate::StaticDelegate<Support::Queue<EasyDelegate::IDeferredCaller*>>(ActionOne));
                phaseOne.push_back(phaseOneGroupOne);

                Support::Vector<SThreadSystem::ThreadAction> phaseTwoGroupOne;
                phaseTwoGroupOne.insert(phaseTwoGroupOne.end(), new EasyDelegate::StaticDelegate<Support::Queue<EasyDelegate::IDeferredCaller*>>(ActionThree));
                phaseTwoGroupOne.insert(phaseTwoGroupOne.end(), new EasyDelegate::StaticDelegate<Support::Queue<EasyDelegate::IDeferredCaller*>>(ActionTwo_PhaseTwo));
                phaseTwo.push_back(phaseTwoGroupOne);

                // Register the phases
                threading->addPhase(phaseOne);
                threading->addPhase(phaseTwo);

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
