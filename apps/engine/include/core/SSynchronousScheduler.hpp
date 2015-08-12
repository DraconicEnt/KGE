/**
 *  @file SSynchronousScheduler.hpp
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

#include <easydelegate/easydelegate.hpp>

#include <support/FTime.hpp>

#include <support/support.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CScheduledEvent
        {
            // Public Methods
            public:
                CScheduledEvent(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Common::U64 &waitTimeMS, const bool &recurring) : mInternalDelegate(cachedDelegate)
                {
                    mTriggerTimeMS = Support::FTime::getSimTimeMilliseconds() + waitTimeMS;

                    mRecurring = recurring;
                    mWaitTimeMS = waitTimeMS;
                    mCancelled = false;
                }

                bool shouldDispatch(const Common::U64 &currentSimTimeMS) NOTHROW
                {
                    return currentSimTimeMS >= mTriggerTimeMS && !mCancelled;
                }

                void setTriggerTimeMS(const Common::U64 &triggerTime)
                {
                    mTriggerTimeMS = triggerTime;
                }

                void dispatch(void)
                {
                    mInternalDelegate->genericDispatch();
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

                const Common::U64 &getWaitTimeMS(void)
                {
                    return mWaitTimeMS;
                }

            // Private Members
            private:
                //! A boolean representing whether or not this scheduled event has been cancelled.
                bool mCancelled;
                //! The time measured in milliseconds at which the scheduled event should be dispatched at.
                Common::U64 mTriggerTimeMS;
                //! The delegate to dispatch when the scheduled event hits its mTriggerTimeMS.
                EasyDelegate::GenericCachedDelegate *mInternalDelegate;

                bool mRecurring;
                Common::U64 mWaitTimeMS;
        };

        class SSynchronousScheduler
        {
            // Public Methods
            public:
                template <typename returnType, typename... parameters>
                CScheduledEvent *schedule(const Common::U32 &waitTimeMS, const bool &recurring, EasyDelegate::StaticMethodPointer<returnType, parameters...> method, parameters... params)
                {
                    return this->schedule(new EasyDelegate::CachedStaticDelegate<returnType, parameters...>(method, params...), waitTimeMS, recurring);
                }

                template <typename classType, typename returnType, typename... parameters>
                CScheduledEvent *schedule(const Common::U32 &waitTimeMS, const bool &recurring, classType *thisPointer, EasyDelegate::MemberMethodPointer<classType, returnType, parameters...> method, parameters... params)
                {
                    return this->schedule(new EasyDelegate::CachedMemberDelegate<classType, returnType, parameters...>(method, thisPointer, params...), waitTimeMS, recurring);
                }

                CScheduledEvent *schedule(EasyDelegate::GenericCachedDelegate *cachedDelegate, const Common::U32 &waitTimeMS, const bool &recurring = false);

                void update(void);

                static SSynchronousScheduler *getPointer(void);

                static void destroy(void);

            // Private Methods
            private:
                //! Standard Constructor.
                SSynchronousScheduler(void) { }
                //! Standard Destructor.
                ~SSynchronousScheduler(void) { }

            // Private Members
            private:
               Support::Set<CScheduledEvent*> mScheduledEventSet;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
