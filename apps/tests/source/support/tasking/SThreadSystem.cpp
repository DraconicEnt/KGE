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
            static void setSecondGameStateValue(const char* value)
            {
                ThirdGameStateValue = const_cast<char*>(value);
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionOne(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, bool>(setFirstGameStateValue, true));

                return transaction;
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionTwo(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, float>(setSecondGameStateValue, 3.14f));

                return transaction;
            }

            static Support::Queue<EasyDelegate::IDeferredCaller*> ActionThree(void)
            {
                Support::Queue<EasyDelegate::IDeferredCaller*> transaction;
                transaction.push(new EasyDelegate::DeferredStaticCaller<void, const char*>(setSecondGameStateValue, "Test String"));

                return transaction;
            }

            TEST(SThreadSystem, Transactions)
            {
                // We initialize the runtime thread count to something predictable
                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();
                settings->setValue<Common::U8>("System::RuntimeThreadCount", 1);

                // Initialize the thread system and two phases
                SThreadSystem* threading = SThreadSystem::getPointer();
                Support::Vector<Support::Vector<SThreadSystem::ThreadAction>> phaseOne;

                // Initialize two groups for phase 1
                Support::Vector<SThreadSystem::ThreadAction> phaseOneGroupOne;
                phaseOneGroupOne.insert(phaseOneGroupOne.end(), new EasyDelegate::StaticDelegate<Support::Queue<EasyDelegate::IDeferredCaller*>>(ActionTwo));
                phaseOneGroupOne.insert(phaseOneGroupOne.end(), new EasyDelegate::StaticDelegate<Support::Queue<EasyDelegate::IDeferredCaller*>>(ActionOne));
                phaseOne.push_back(phaseOneGroupOne);

                // Register the phases
                threading->addPhase(phaseOne);

                // We wait a little bit for our actions to process
                for (Common::U32 iteration = 0; iteration < 4; iteration++)
                {
                    threading->update();

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }

                Support::SSettingsRegistry::destroy();
                SThreadSystem::destroy();
            }
        }
    } // End Namespace Support
} // End namespace Kiaro
