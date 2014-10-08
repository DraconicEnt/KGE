/**
 *  @file Time.hpp
 *  @brief Include file for time functionality.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.0
 *  @date 3/19/2014
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
            Kiaro::Common::F32 stopTimer(timer id);

            Kiaro::Common::U64 getCurrentTimeMicroseconds(void);
            Kiaro::Common::U64 getTimerResolutionMicroseconds(void);

            Kiaro::Common::U64 getSimTimeMicroseconds(void);
            Kiaro::Common::U64 getSimTimeMilliseconds(void);
        } // End NameSpace Time
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_TIME_HPP_
