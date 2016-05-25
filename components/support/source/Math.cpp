/**
 */

#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        Common::F32 FPSToMS(const Common::F32 fps)
        {
            return 1000.0f / fps;
        }
    }
}
