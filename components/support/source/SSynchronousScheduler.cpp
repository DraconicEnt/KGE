/**
 *  @file SSynchronousScheduler.cpp
 *  @brief Source file implementing the scheduler system.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @ref http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
 *
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <support/SSynchronousScheduler.hpp>

namespace Kiaro
{
    namespace Support
    {
        static SSynchronousScheduler* sInstance = NULL;

        CScheduledEvent::CScheduledEvent(EasyDelegate::IDeferredCaller* deferredCaller, const Common::U64& waitTimeMS, const bool& recurring) : mInternalDeferredCaller(deferredCaller)
        {
            mTriggerTimeMS = Support::FTime::getSimTimeMilliseconds() + waitTimeMS;

            mRecurring = recurring;
            mWaitTimeMS = waitTimeMS;
            mCancelled = false;
        }

        bool CScheduledEvent::shouldDispatch(const Common::U64& currentSimTimeMS)
        {
            return currentSimTimeMS >= mTriggerTimeMS && !mCancelled;
        }

        void CScheduledEvent::setTriggerTimeMS(const Common::U64& triggerTime)
        {
            mTriggerTimeMS = triggerTime;
        }

        void CScheduledEvent::dispatch(void)
        {
            mInternalDeferredCaller->genericDispatch();
        }

        void CScheduledEvent::cancel(void)
        {
            mCancelled = true;
        }

        const bool& CScheduledEvent::isCancelled(void)
        {
            return mCancelled;
        }

        const bool& CScheduledEvent::isRecurring(void)
        {
            return mRecurring;
        }

        const Common::U64& CScheduledEvent::getWaitTimeMS(void)
        {
            return mWaitTimeMS;
        }

        SSynchronousScheduler* SSynchronousScheduler::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SSynchronousScheduler;

            return sInstance;
        }

        void SSynchronousScheduler::destroy(void)
        {
            if (sInstance)
                delete sInstance;

            sInstance = NULL;
        }

        CScheduledEvent* SSynchronousScheduler::schedule(EasyDelegate::IDeferredCaller* deferredCaller, const Common::U32& waitTimeMS, const bool& recurring)
        {
            CScheduledEvent* event = new CScheduledEvent(deferredCaller, waitTimeMS, recurring);
            mScheduledEventSet.insert(event);

            return event;
        }

        void SSynchronousScheduler::update(void)
        {
            const Common::U64 currentSimTimeMS = Support::FTime::getSimTimeMilliseconds();

            for (auto iterator = mScheduledEventSet.begin(); iterator != mScheduledEventSet.end(); iterator++)
            {
                bool shouldRemoveCurrentEvent = false;
                CScheduledEvent* currentEvent = *iterator;

                if (currentEvent->shouldDispatch(currentSimTimeMS))
                {
                    currentEvent->dispatch();

                    if (currentEvent->isRecurring())
                        currentEvent->setTriggerTimeMS(Support::FTime::getSimTimeMilliseconds() + currentEvent->getWaitTimeMS());
                    else
                        shouldRemoveCurrentEvent = true;
                }
                else if (currentEvent->isCancelled())
                    shouldRemoveCurrentEvent = true;

                // FIXME: Breaks the iterator?
                if (shouldRemoveCurrentEvent)
                {
                    mScheduledEventSet.erase(currentEvent);
                    delete currentEvent;
                }
            }
        }
    } // End Namespace Support
} // End Namespace Kiaro
