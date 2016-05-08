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
				LARGE_INTEGER time, frequency;
				QueryPerformanceFrequency(&frequency);
				QueryPerformanceCounter(&time);

				time.QuadPart *= 1000000;
				time.QuadPart /= frequency.QuadPart;

				return time.QuadPart;
			}
		}
	}
}