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
            //! File access mode.
            enum ACCESS_MODE
            {
                READ = 1,
                WRITE = 2,
                APPEND = 4,
            };

            /**
             *  @brief Determines whether or not a file by the given name exists.
             *  @param filename The path to the file to check.
             *  @return A boolean representing whether or not that file exists.
             */
            static bool exists(const Support::String& filename)
            {
                FILE* handle = fopen(filename.data(), "r");
                if (!handle)
                    return false;

                fclose(handle);
                return true;
            }

            /**
             *  @brief A wrapper around virtual memory space -> file contents wrapping to abstract away the platform
             *  differences in performing this operation.
             */
            class MemoryMappedFile
            {
                public:
                    //! The start of the virtual memory space that the access starts at.
                    void* mAddress;

                    //! The length of the mapped memory space.
                    size_t mLength;

                    //! The file that this memory is mapped to.
                    const Support::String mFilePath;

                    //! The access mode bitmask in use for this.
                    const ACCESS_MODE mAccessMode;

                    //! The offset into the file to map out.
                    size_t mOffset;

                public:
                    /**
                     *  @brief A constructor accepting a file path.
                     *  @param The file path to bind against.
                     */
                    MemoryMappedFile(const Support::String& filepath, const ACCESS_MODE& mode);

                    //! Standard destructor.
                    ~MemoryMappedFile(void);

                    /**
                     *  @brief Opens up the file to be mapped into memory.
                     */
                    void open(void);
                    void close(void);

                private:
					//! A pointer to arbitrary data that may be used however necessary for platform implementations.
					void* mData;

                    //! A pointer to the internal file we're using for the memory mapping.
                    FILE* mHandle;
            };
        }
    }
}
#endif // _INCLUDE_PLATFORM_HPP_
