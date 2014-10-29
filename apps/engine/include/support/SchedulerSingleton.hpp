/**
 *  @file Scheduler.hpp
 *  @brief Include file defining endian conversion functionality.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @ref http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
 *
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <tuple>

#include <easydelegate.hpp>

#include <support/Time.hpp>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class ScheduledEvent
        {
            public:
                ScheduledEvent(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U64 &waitTimeMS) : mInternalDelegate(cachedDelegate)
                {
                    mTriggerTimeMS = Kiaro::Support::Time::getSimTimeMilliseconds() + waitTimeMS;
                }

                bool shouldDispatch(const Kiaro::Common::U64 &currentSimTimeMS)
                {
                    return currentSimTimeMS >= mTriggerTimeMS && !mIsCancelled;
                }

                void dispatch(void)
                {
                    mInternalDelegate->generic_dispatch();
                }

                void cancel(void)
                {
                    mIsCancelled = true;
                }

                bool isCancelled(void)
                {
                    return mIsCancelled;
                }

            // Private Members
            private:
                bool mIsCancelled;
                Kiaro::Common::U64 mTriggerTimeMS;
                EasyDelegate::GenericCachedDelegate *mInternalDelegate;
        };

        class SchedulerSingleton
        {
            public:
                ScheduledEvent *schedule(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U32 &waitTimeMS);

                void update(void);

                static SchedulerSingleton *getPointer(void);

                static void destroy(void);

            private:
                //! Standard Constructor.
                SchedulerSingleton(void) { }
                //! Standard Destructor.
                ~SchedulerSingleton(void) { }

                std::set<ScheduledEvent*> mScheduledEventSet;
        }; // End class Scheduler
    } // End NameSpace Support
} // End NameSpace Kiaro
