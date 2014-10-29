/**
 *  @file SchedulerSingleton.cpp
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

#include <support/SchedulerSingleton.hpp>

namespace Kiaro
{
    namespace Support
    {
        static SchedulerSingleton *SchedulerSingleton_Instance = NULL;

        SchedulerSingleton *SchedulerSingleton::getPointer(void)
        {
            if (!SchedulerSingleton_Instance)
                SchedulerSingleton_Instance = new SchedulerSingleton;

            return SchedulerSingleton_Instance;
        }

        void SchedulerSingleton::destroy(void)
        {
            if (SchedulerSingleton_Instance)
                delete SchedulerSingleton_Instance;

            SchedulerSingleton_Instance = NULL;
        }

        ScheduledEvent *SchedulerSingleton::schedule(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Kiaro::Common::U32 &waitTimeMS)
        {
            ScheduledEvent *event = new ScheduledEvent(cachedDelegate, waitTimeMS);
            mScheduledEventSet.insert(event);

            return event;
        }

        void SchedulerSingleton::update(void)
        {
            const Kiaro::Common::U64 &currentSimTimeMS = Kiaro::Support::Time::getSimTimeMilliseconds();

            for (std::set<ScheduledEvent*>::iterator iterator = mScheduledEventSet.begin(); iterator != mScheduledEventSet.end(); iterator++)
            {
                bool shouldRemoveCurrentEvent = false;
                ScheduledEvent *currentEvent = *iterator;

                if (currentEvent->shouldDispatch(currentSimTimeMS))
                {
                    currentEvent->dispatch();
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
