/**
 *  @file SSynchronousScheduler.hpp
 *  @brief Include file defining the scheduler system.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @ref http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
 *
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_SUPPORT_SSYNCHRONOUSSCHEDULER_HPP_
#define _INCLUDE_SUPPORT_SSYNCHRONOUSSCHEDULER_HPP_

#include <support/Tuple.hpp>

#include <easydelegate/easydelegate.hpp>

#include <support/FTime.hpp>

#include <support/support.hpp>
#include <support/common.hpp>
#include <support/ISingleton.hpp>
#include <support/UnorderedSet.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief A class representing an event that has been scheduled to occur at a later time within the
         *  current game session.
         */
        class CScheduledEvent
        {
                // Private Members
            private:
                //! A boolean representing whether or not this scheduled event has been cancelled.
                bool mCancelled;
                //! The time measured in milliseconds at which the scheduled event should be dispatched at.
                Common::U64 mTriggerTimeMS;
                //! The delegate to dispatch when the scheduled event hits its mTriggerTimeMS.
                EasyDelegate::IDeferredCaller* mInternalDeferredCaller;

                //! Determines whether or not this scheduled event is recurring.
                bool mRecurring;
                //! How much time to wait in milliseconds before dispatching.
                Common::U64 mWaitTimeMS;

                // Public Methods
            public:
                /**
                 *  @brief Constructor accepting a deferred caller, the time to wait and a recurring state.
                 *  @param deferredCaller A pointer to the deferred caller to use internally.
                 *  @param waitTimeMS The time to wait in milliseconds relative to the current time.
                 *  @param recurring A boolean representing whether or not the scheduled event is recurring.
                 */
                CScheduledEvent(EasyDelegate::IDeferredCaller* deferredCaller, const Common::U64 waitTimeMS, const bool recurring);

                ~CScheduledEvent(void);

                /**
                 *  @brief Returns whether or not the scheduled event should be dispatched given
                 *  the current time.
                 *  @return A boolean representing whether or not the scheduled event should be
                 *  dispatched.
                 */
                bool shouldDispatch(const Common::U64 currentSimTimeMS) NOTHROW;

                /**
                 *  @brief Sets the absolute trigger time for the scheduled event. This means
                 *  that the scheduled event will execute once the sim time meets or exceeds
                 *  the given trigger time.
                 *  @param triggerTime The absolute sim time in milliseconds to trigger upon meeting
                 *  or exceeding.
                 */
                void setTriggerTimeMS(const Common::U64 triggerTime) NOTHROW;

                /**
                 *  @brief
                 */
                void setWaitTimeMS(const Common::U64 waitTimeMS, const bool refresh) NOTHROW;

                /**
                 *  @brief Dispatches the scheduled event, calling whatever code it is set
                 *  to call.
                 *  @throw std::exception Anything can be thrown during the processing of the
                 *  scheduled code.
                 */
                void dispatch(void);

                /**
                 *  @brief Flags the scheduled event for cancellation. It will be removed on the
                 *  next iteration of the scheduler.
                 */
                void cancel(void) NOTHROW;

                /**
                 *  @brief Returns whether or not the scheduled event was flagged for cancellation
                 *  and eventual removal.
                 *  @return A boolean representing whether or not the scheduled event was flagged for
                 *  cancellation.
                 */
                const bool isCancelled(void) NOTHROW;

                /**
                 *  @brief Returns whether or not this scheduled event is flagged to operate on a
                 *  recurring basis. This means the event is never removed by the scheduler and
                 *  must be removed manually because it is executed once per mWaitTimeMS.
                 *  @return A boolean representing whether or the scheduled event is flagged to operate
                 *  on a recurring basis.
                 */
                const bool isRecurring(void) NOTHROW;

                /**
                 *  @brief Returns the time delta that the scheduled event is set to execute on.
                 *  If it is a recurring event, this is the time delta between executions.
                 *  @return A Common::U64 representing the wait time in milliseconds.
                 */
                const Common::U64 getWaitTimeMS(void) NOTHROW;
        };

        /**
         *  @brief A scheduler singleton used to automate deferred and recurring calls within the context
         *  of the main thread, hence the name SSynchronousScheduler. In contrast, there is the SAsynchronousScheduler
         *  for accurate timings.
         */
        class SSynchronousScheduler : public ISingleton<SSynchronousScheduler>
        {
                // Private Members
            private:
                //! A set of scheduled events for processing.
                Support::UnorderedSet<CScheduledEvent*> mScheduledEventSet;

                // Public Methods
            public:
                /**
                 *  @brief Schedules a call using a pointer to a class member method.
                 *  @param waitTimeMS The time in milliseconds to wait relative to the current time.
                 *  @param recurring An optional boolean that defaults to false which is used to control the recurring
                 *  behavior of the scheduled event.
                 *  @param method A pointer to the static method to be dispatching.
                 *  @param params... All other parameters beyond the class method pointer will be used as parameters when the scheduled
                 *  event is processed.
                 *  @return A pointer to the created event.
                 *  @warning If recurring is true, then the returned CScheduledEvent should be kept track of in almost all cases.
                 */
                template <typename returnType, typename... parameters>
                CScheduledEvent* schedule(const Common::U32 waitTimeMS, const bool recurring, EasyDelegate::StaticMethodPointer<returnType, parameters...> method, parameters... params)
                {
                    return this->schedule(new EasyDelegate::DeferredStaticCaller<returnType, parameters...>(method, params...), waitTimeMS, recurring);
                }

                /**
                 *  @brief Schedules a call using a pointer to a class member method.
                 *  @param waitTimeMS The time in milliseconds to wait relative to the current time.
                 *  @param recurring An optional boolean that defaults to false which is used to control the recurring
                 *  behavior of the scheduled event.
                 *  @param thisPointer A pointer to the object to be used as "this" when dispatching the deferred call internally.
                 *  @param method A pointer to the class member method to be dispatching.
                 *  @param params... All other parameters beyond the class method pointer will be used as parameters when the scheduled
                 *  event is processed.
                 *  @return A pointer to the created event.
                 *  @warning If recurring is true, then the returned CScheduledEvent should be kept track of in almost all cases.
                 *  @warning The thisPointer parameter must be guaranteed to be valid for the lifetime of the scheduled event.
                 */
                template <typename classType, typename returnType, typename... parameters>
                CScheduledEvent* schedule(const Common::U32 waitTimeMS, const bool recurring, classType* thisPointer, EasyDelegate::MemberMethodPointer<classType, returnType, parameters...> method, parameters... params)
                {
                    return this->schedule(new EasyDelegate::DeferredMemberCaller<classType, returnType, parameters...>(method, thisPointer, params...), waitTimeMS, recurring);
                }

                /**
                 *  @brief Schedules a call using an IDeferredCaller pointer.
                 *  @param deferredCaller A pointer to the deferred caller to schedule.
                 *  @param waitTimeMS The time in milliseconds to wait relative to the current time.
                 *  @param recurring An optional boolean that defaults to false which is used to control the recurring
                 *  behavior of the scheduled event.
                 *  @return A pointer to the created event.
                 *  @warning If recurring is true, then the returned CScheduledEvent should be kept track of in almost all cases.
                 */
                CScheduledEvent* schedule(EasyDelegate::IDeferredCaller* deferredCaller, const Common::U32 waitTimeMS, const bool recurring = false);

                /**
                 *  @brief Sends an update time pulse to the SSynchronousScheduler singleton. Call this once
                 *  per main loop iteration.
                 */
                void update(void);
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_SUPPORT_SSYNCHRONOUSSCHEDULER_HPP_
