/**
 *  @file SExtensionLoader.cpp
 *  @brief Unix specific programming for the extension loader.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

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

