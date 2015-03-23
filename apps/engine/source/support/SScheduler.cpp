/**
 *  @file SchedulerSingleton.cpp
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

#include <engine/SScheduler.hpp>

namespace Kiaro
{
    namespace Support
    {
        static SScheduler *SchedulerSingleton_Instance = NULL;

        SScheduler *SScheduler::getPointer(void)
        {
            if (!SchedulerSingleton_Instance)
                SchedulerSingleton_Instance = new SScheduler;

            return SchedulerSingleton_Instance;
        }

        void SScheduler::destroy(void)
        {
            if (SchedulerSingleton_Instance)
                delete SchedulerSingleton_Instance;

            SchedulerSingleton_Instance = NULL;
        }

        CScheduledEvent *SScheduler::schedule(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U32 &waitTimeMS, const bool &recurring)
        {
            CScheduledEvent *event = new CScheduledEvent(cachedDelegate, waitTimeMS, recurring);
            mScheduledEventSet.insert(event);

            return event;
        }

        void SScheduler::update(void)
        {
            const Kiaro::Common::U64 currentSimTimeMS = Kiaro::Support::FTime::getSimTimeMilliseconds();

            for (std::set<CScheduledEvent *>::iterator iterator = mScheduledEventSet.begin(); iterator != mScheduledEventSet.end(); iterator++)
            {
                bool shouldRemoveCurrentEvent = false;
                CScheduledEvent *currentEvent = *iterator;

                if (currentEvent->shouldDispatch(currentSimTimeMS))
                {
                    currentEvent->dispatch();

                    if (currentEvent->isRecurring())
                        currentEvent->setTriggerTimeMS(Kiaro::Support::FTime::getSimTimeMilliseconds() + currentEvent->getWaitTimeMS());
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
