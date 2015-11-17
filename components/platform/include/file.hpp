/**
 */

#include <platform/file.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            struct MemoryMapppedFile
            {
                FILE* handle;
                void* address;
            };

            static MemoryMapppedFile* OpenMemoryMap(const Support::String& filepath);
            static void CloseMemoryMap(MemoryMappedFile* mapped);
        }
    }
}
