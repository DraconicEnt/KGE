/**
 *  @file SchedulerSingleton.hpp
 *  @brief Include file defining the scheduler system.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @ref http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
 *
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <tuple> // std::tuple

#include <easydelegate.hpp>

#include <support/FTime.hpp>

#include <engine/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CScheduledEvent
        {
            // Public Methods
            public:
                CScheduledEvent(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U64 &waitTimeMS, const bool &recurring) : mInternalDelegate(cachedDelegate)
                {
                    mTriggerTimeMS = Kiaro::Support::FTime::getSimTimeMilliseconds() + waitTimeMS;

                    mRecurring = recurring;
                    mWaitTimeMS = waitTimeMS;
                    mCancelled = false;
                }

                bool shouldDispatch(const Kiaro::Common::U64 &currentSimTimeMS) NOTHROW
                {
                    return currentSimTimeMS >= mTriggerTimeMS && !mCancelled;
                }

                void setTriggerTimeMS(const Kiaro::Common::U64 &triggerTime)
                {
                    mTriggerTimeMS = triggerTime;
                }

                void dispatch(void)
                {
                    mInternalDelegate->generic_dispatch();
                }

                void cancel(void) NOTHROW
                {
                    mCancelled = true;
                }

                const bool &isCancelled(void) NOTHROW
                {
                    return mCancelled;
                }

                const bool &isRecurring(void)
                {
                    return mRecurring;
                }

                const Kiaro::Common::U64 &getWaitTimeMS(void)
                {
                    return mWaitTimeMS;
                }

            // Private Members
            private:
                //! A boolean representing whether or not this scheduled event has been cancelled.
                bool mCancelled;
                //! The time measured in milliseconds at which the scheduled event should be dispatched at.
                Kiaro::Common::U64 mTriggerTimeMS;
                //! The delegate to dispatch when the scheduled event hits its mTriggerTimeMS.
                EasyDelegate::GenericCachedDelegate *mInternalDelegate;

                bool mRecurring;
                Kiaro::Common::U64 mWaitTimeMS;
        };

        class SScheduler
        {
            // Public Methods
            public:
                CScheduledEvent *schedule(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U32 &waitTimeMS, const bool &recurring = false);

                void update(void);

                static SScheduler *getPointer(void);

                static void destroy(void);

            // Private Methods
            private:
                //! Standard Constructor.
                SScheduler(void) { }
                //! Standard Destructor.
                ~SScheduler(void) { }

            // Private Members
            private:
                std::set<CScheduledEvent*> mScheduledEventSet;
        }; // End class Scheduler
    } // End NameSpace Support
} // End NameSpace Kiaro
