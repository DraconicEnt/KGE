#include <Windows.h>

#include <support/platform/time.hpp>

namespace Kiaro
{
	namespace Platform
	{
		namespace Time
		{
			Common::U64 getCurrentTimeMicroseconds(void)
			{
				LARGE_INTEGER ticksPerSecond;
				LARGE_INTEGER tick;
				LARGE_INTEGER time;

				QueryPerformanceFrequency(&ticksPerSecond);
				QueryPerformanceCounter(&tick);

				Common::U64 ticks_per_micro = ticksPerSecond.QuadPart / 1000000;
				return (tick.QuadPart % ticksPerSecond.QuadPart) / ticks_per_micro;
			}
		}
	}
}