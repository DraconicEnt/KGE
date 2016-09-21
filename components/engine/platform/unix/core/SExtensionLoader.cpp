/**
 */

// FIXME: Implement in a cross-platform manner
#include <dlfcn.h>

#include <core/SExtensionLoader.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            SExtensionLoader::ExtensionSymbols SExtensionLoader::internalLoadExtension(const Support::String& filename)
            {
                SExtensionLoader::ExtensionSymbols symbols;
                symbols.mHandle = nullptr;

                void* handle = dlopen(filename.data(), RTLD_LAZY);

                if (!handle)
                    return symbols;

                symbols.mHandle = handle;
                symbols.mInitialize = reinterpret_cast<ExtensionSymbols::extensionInitializePointer>(dlsym(handle, "extensionInitialize"));
                symbols.mDeinitialize = reinterpret_cast<ExtensionSymbols::extensionInitializePointer>(dlsym(handle, "extensionDeinitialize"));

                // Invalid symbols, we can't do jack in this case.
                if (!symbols.mInitialize || !symbols.mDeinitialize)
                {
                    dlclose(handle);
                    return symbols;
                }

                return symbols;
            }

            void SExtensionLoader::internalUnloadExtension(SExtensionLoader::ExtensionSymbols* symbols)
            {
                dlclose(symbols->mHandle);
            }
        }
    }
}

