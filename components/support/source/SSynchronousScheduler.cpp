/**
 *  @file SSynchronousScheduler.cpp
 *  @brief Source file implementing the scheduler system.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @ref http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/SSynchronousScheduler.hpp>

namespace Kiaro
{
    namespace Support
    {
        static SSynchronousScheduler* sInstance = nullptr;

        CScheduledEvent::CScheduledEvent(EasyDelegate::IDeferredCaller* deferredCaller, const Common::U64 waitTimeMS, const bool recurring) :
            mCancelled(false), mTriggerTimeMS(Support::FTime::getSimTimeMilliseconds() + waitTimeMS), mInternalDeferredCaller(deferredCaller),
            mRecurring(recurring), mWaitTimeMS(waitTimeMS)
        {
        }

        CScheduledEvent::~CScheduledEvent(void)
        {
            delete mInternalDeferredCaller;
            mInternalDeferredCaller = nullptr;
        }

        bool CScheduledEvent::shouldDispatch(const Common::U64 currentSimTimeMS)
        {
            return currentSimTimeMS >= mTriggerTimeMS && !mCancelled;
        }

        void CScheduledEvent::setTriggerTimeMS(const Common::U64 triggerTime)
        {
            mTriggerTimeMS = triggerTime;
        }

        void CScheduledEvent::setWaitTimeMS(const Common::U64 waitTimeMS, const bool refresh)
        {
            mWaitTimeMS = waitTimeMS;

            if (refresh)
                mTriggerTimeMS = Support::FTime::getSimTimeMilliseconds() + waitTimeMS;
        }

        void CScheduledEvent::dispatch(void)
        {
            mInternalDeferredCaller->genericDispatch();
        }

        void CScheduledEvent::cancel(void)
        {
            mCancelled = true;
        }

        bool CScheduledEvent::isCancelled(void)
        {
            return mCancelled;
        }

        bool CScheduledEvent::isRecurring(void)
        {
            return mRecurring;
        }

        Common::U64 CScheduledEvent::getWaitTimeMS(void)
        {
            return mWaitTimeMS;
        }

        SSynchronousScheduler::SSynchronousScheduler(void)
        {

        }

        SSynchronousScheduler::~SSynchronousScheduler(void)
        {

        }

        CScheduledEvent* SSynchronousScheduler::schedule(EasyDelegate::IDeferredCaller* deferredCaller, const Common::U32 waitTimeMS, const bool recurring)
        {
            return *mScheduledEventSet.insert(mScheduledEventSet.end(), new CScheduledEvent(deferredCaller, waitTimeMS, recurring));
        }

        void SSynchronousScheduler::update(void)
        {
            const Common::U64 currentSimTimeMS = Support::FTime::getSimTimeMilliseconds();
            Support::UnorderedSet<CScheduledEvent*> removedEvents;

            for (CScheduledEvent* currentEvent : mScheduledEventSet)
            {
                bool shouldRemoveCurrentEvent = false;

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

                if (shouldRemoveCurrentEvent)
                    removedEvents.insert(currentEvent);
            }

            for (CScheduledEvent* currentEvent : removedEvents)
            {
                mScheduledEventSet.erase(currentEvent);
                delete currentEvent;
            }
        }
    } // End Namespace Support
} // End Namespace Kiaro
