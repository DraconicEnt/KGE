/**
 *  @file Time.hpp
 *  @brief Include file defining timing functionality.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_TIME_HPP_
#define _INCLUDE_KIARO_SUPPORT_TIME_HPP_

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <core/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace FTime
        {
            typedef Common::U8 timer;

            timer startTimer(void);

            /**
             *  @throw std::logic_error When the given timerIdentifier is not equal
             *  to that of the current top level timer identifier.
             *  @throw std::logic_error When there are no running timers to stop.
             */
            Common::F32 stopTimer(const timer &timerIdentifier);

            Common::U64 getCurrentTimeMicroseconds(void) NOTHROW;
            Common::U64 getTimerResolutionMicroseconds(void) NOTHROW;

            Common::U64 getSimTimeMicroseconds(void) NOTHROW;
            Common::U64 getSimTimeMilliseconds(void) NOTHROW;
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_TIME_HPP_
