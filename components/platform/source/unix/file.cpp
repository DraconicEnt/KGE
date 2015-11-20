/**
 */

#include <sys/mman.h>

#include <platform/file.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            MemoryMappedFile::MemoryMappedFile(const Support::String& filepath)
            {
                this->handle = fopen(filepath.data(), "r");
                this->address = mmap(NULL, 0, PROT_READ, MAP_PRIVATE, fileno(handle), 0);
            }

            MemoryMappedFile::~MemoryMappedFile(void)
            {
                munmap(this->address, 0);
                fclose(this->handle);
            }
        }
    }
}
