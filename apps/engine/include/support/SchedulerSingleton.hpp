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

#include <support/Time.hpp>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class ScheduledEvent
        {
            // Public Methods
            public:
                ScheduledEvent(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U64 &waitTimeMS) : mInternalDelegate(cachedDelegate)
                {
                    mTriggerTimeMS = Kiaro::Support::Time::getSimTimeMilliseconds() + waitTimeMS;
                }

                bool shouldDispatch(const Kiaro::Common::U64 &currentSimTimeMS) NOTHROW
                {
                    return currentSimTimeMS >= mTriggerTimeMS && !mIsCancelled;
                }

                void dispatch(void)
                {
                    mInternalDelegate->generic_dispatch();
                }

                void cancel(void) NOTHROW
                {
                    mIsCancelled = true;
                }

                bool isCancelled(void) NOTHROW
                {
                    return mIsCancelled;
                }

            // Private Members
            private:
                //! A boolean representing whether or not this scheduled event has been cancelled.
                bool mIsCancelled;
                //! The time measured in milliseconds at which the scheduled event should be dispatched at.
                Kiaro::Common::U64 mTriggerTimeMS;
                //! The delegate to dispatch when the scheduled event hits its mTriggerTimeMS.
                EasyDelegate::GenericCachedDelegate *mInternalDelegate;
        };

        class SchedulerSingleton
        {
            // Public Methods
            public:
                ScheduledEvent *schedule(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U32 &waitTimeMS);

                void update(void);

                static SchedulerSingleton *getPointer(void);

                static void destroy(void);

            // Private Methods
            private:
                //! Standard Constructor.
                SchedulerSingleton(void) { }
                //! Standard Destructor.
                ~SchedulerSingleton(void) { }
                
            // Private Members
            private:
                std::set<ScheduledEvent*> mScheduledEventSet;
        }; // End class Scheduler
    } // End NameSpace Support
} // End NameSpace Kiaro
