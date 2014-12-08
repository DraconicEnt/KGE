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

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        namespace Time
        {
            typedef Kiaro::Common::U8 timer;

            timer startTimer(void);
            Kiaro::Common::F32 stopTimer(const timer &timerIdentifier);

            Kiaro::Common::U64 getCurrentTimeMicroseconds(void) NOTHROW;
            Kiaro::Common::U64 getTimerResolutionMicroseconds(void) NOTHROW;

            Kiaro::Common::U64 getSimTimeMicroseconds(void) NOTHROW;
            Kiaro::Common::U64 getSimTimeMilliseconds(void) NOTHROW;
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_TIME_HPP_
