#include <sys/time.h>

#include <support/platform/time.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace Time
        {
            Common::U64 getCurrentTimeMicroseconds(void)
            {
                timeval currentTime;
                gettimeofday(&currentTime, NULL);
                return currentTime.tv_usec + (1000000ULL * currentTime.tv_sec);
            }
        }
    }
}
