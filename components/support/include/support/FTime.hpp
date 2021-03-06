/**
 *  @file FTime.hpp
 *  @brief Include file declaring timing functionality.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_SUPPORT_TIME_HPP_
#define _INCLUDE_KIARO_SUPPORT_TIME_HPP_

// #include <boost/date_time/posix_time/posix_time_types.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            typedef Common::U8 timer;

            /**
             *  @brief Starts a new timer and returns the identifier to it to be used in stopTimer.
             *  @return The identifier of the timer that was started.
             */
            timer startTimer(void);

            /**
             *  @throw std::logic_error When the given timerIdentifier is not equal
             *  to that of the current top level timer identifier.
             *  @throw std::logic_error When there are no running timers to stop.
             */
            Common::F32 stopTimer(const timer& timerIdentifier);

            /**
             *  @brief Clears all timers in the timing system.
             */
            void clearTimers(void);

            Common::U64 getSimTimeMilliseconds(void) NOTHROW;
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_TIME_HPP_
