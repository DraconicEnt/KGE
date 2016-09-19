/**
 *  @file SExtensionLoader.hpp
 */

#ifndef _INCLUDE_CORE_SEXTENSIONLOADER_HPP_
#define _INCLUDE_CORE_SEXTENSIONLOADER_HPP_

#include <physfs.h>

#include <support/String.hpp>

#include <support/UnorderedMap.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            /**
             *  @brief A singleton extension loader used for managing the loadable extensions into the engine.
             *  @detail These extensions are merely shared object files we load into the game memory and can reload or unload at will.
             *  They must conform to an expected standard in order to be loadable, which primarily regards having several static
             *  methods declared and loadable through dlsym.
             */
            class SExtensionLoader
            {
                // Public Members
                public:
                    //! A structure used for storing the extension symbols.
                    struct ExtensionSymbols
                    {
                        void* mHandle;

                        typedef void (*extensionInitializePointer)(void);
                        typedef void (*extensionDeinitializePointer)(void);

                        extensionInitializePointer mInitialize;
                        extensionDeinitializePointer mDeinitialize;
                    };

                // Private Members
                private:
                    //! An unordered map mapping extension paths to the extension handle.
                    Support::UnorderedMap<Support::String, ExtensionSymbols> mExtensionMap;

                // Public Methods
                public:
                    static SExtensionLoader* getPointer(void);
                    static void destroy(void);

                    bool loadExtension(const Support::String& filename);
                    bool unloadExtension(const Support::String& filename);
                    bool reloadExtension(const Support::String& filename);

                // Private Methods
                private:
                    //! Parameter-less constructor.
                    SExtensionLoader(void);

                    //! Standard destructor.
                    ~SExtensionLoader(void);

                    void deleteExtension(const Support::String& filename);
                    ExtensionSymbols* findExtension(const Support::String& filename);
            };
        }
    }
}
#endif // _INCLUDE_CORE_SEXTENSIONLOADER_HPP_
