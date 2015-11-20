/**
 */

#ifndef _INCLUDE_PLATFORM_HPP_
#define _INCLUDE_PLATFORM_HPP_

#include <cstdio>

#include <support/String.hpp>

namespace Kiaro
{
    namespace Platform
    {
        namespace File
        {
            /**
             *  @brief A wrapper around virtual memory space -> file contents wrapping to abstract away the platform
             *  differences in performing this operation.
             */
            class MemoryMappedFile
            {
                public:
                    //! The start of the virtual memory space that the access starts at.
                    void* address;

                public:
                    /**
                     *  @brief A constructor accepting a file path.
                     *  @param The file path to bind against.
                     */
                    MemoryMappedFile(const Support::String& filepath);

                    //! Standard destructor.
                    ~MemoryMappedFile(void);

                private:
                    //! A pointer to the internal file we're using for the memory mapping.
                    FILE* handle;
            };
        }
    }
}
#endif // _INCLUDE_PLATFORM_HPP_
