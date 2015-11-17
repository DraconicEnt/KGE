/**
 */

#include <sys/mman.h>

#include <support/String.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            static MemoryMapppedFile* OpenMemoryMap(const Support::String& filepath);
            {
                FILE* handle = fopen(filepath.data(), "r");
                void* address = mmap(NULL, 0, PROT_READ, MAP_PRIVATE, handle, 0);

                MemoryMappedFile* result = new MemoryMappedFile();
                result->handle = handle;
                result->address = address;
            }

            static void CloseMemoryMap(MemoryMappedFile* mapped);
            {
                munmap(mapped->address, 0);
                fclose(mapped->handle);
            }
        }
    }
}
