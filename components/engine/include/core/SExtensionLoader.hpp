/**
 *  @file SExtensionLoader.hpp
 *  @brief Include file defining the extension loader singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CORE_SEXTENSIONLOADER_HPP_
#define _INCLUDE_CORE_SEXTENSIONLOADER_HPP_

#include <physfs.h>

#include <support/String.hpp>
#include <support/ISingleton.hpp>
#include <support/UnorderedMap.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            /**
             *  @brief A singleton extension loader used for managing the loadable extensions into the engine.
             *  @details These extensions are merely shared object files we load into the game memory and can reload or unload at will.
             *  They must conform to an expected standard in order to be loadable, which primarily regards having several static
             *  methods declared and loadable through dlsym.
             */
            class SExtensionLoader : public Support::ISingleton<SExtensionLoader>
            {
                // Public Members
                public:
                    //! A structure used for storing the extension symbols.
                    struct ExtensionSymbols
                    {
                        //! Poinder to the operating system handle for the extension.
                        void* mHandle;

                        //! Typedef for a method pointer to the extension initialization routine.
                        typedef void (*extensionInitializePointer)(void);
                        //! Typedef for a method pointer to the extension deinitialization routine.
                        typedef void (*extensionDeinitializePointer)(void);

                        //! The extension initialization routine.
                        extensionInitializePointer mInitialize;
                        //! The extension deinitialization routine.
                        extensionDeinitializePointer mDeinitialize;
                    };

                // Private Members
                private:
                    //! An unordered map mapping extension paths to the extension handle.
                    Support::UnorderedMap<Support::String, ExtensionSymbols> mExtensionMap;

                // Public Methods
                public:
                    /**
                     *  @brief Loads the specified extension by filename, adhering to the rules of the VFS.
                     *  @param filename The path to the extension to load within the confines of the VFS.
                     *  @return True for successful. False otherwise.
                     */
                    bool loadExtension(const Support::String& filename);

                    /**
                     *  @brief Totally unloads the specified extension from the engine runtime.
                     *  @param filename The name of the extension to unload.
                     *  @return True for successful. False otherwise.
                     */
                    bool unloadExtension(const Support::String& filename);

                    /**
                     *  @brief Reloads the specified extension from disk. This is useful for when there's programming to be updated
                     *  from an extension. Ie: You are developing engine code using an extension to see changes in almost realtime.
                     *  @param filename The name of the extension to reload.
                     *  @return True for success. False otherwise.
                     */
                    bool reloadExtension(const Support::String& filename);

                // Private Methods
                private:
                    /**
                     *  @brief Platform specific programming to load the extension by the given filename. This is implemented on a per
                     *  platform basis in engine/platform.
                     *  @param filename The filename of the extension to load.
                     */
                    ExtensionSymbols internalLoadExtension(const Support::String& filename);

                    /**
                     *  @brief Platform specific programming to unload the extension by the given filename. This is implemented on a per
                     *  platform basis in engine/platform.
                     *  @param filename The filename of the extension to unload.
                     */
                    void internalUnloadExtension(ExtensionSymbols* symbols);

                    /**
                     *  @brief Helper method to delete the extension from our loaded extension registry.
                     *  @param filename The filename of the extension to delete from the runtime extension registry.
                     */
                    void deleteExtension(const Support::String& filename);

                    /**
                     *  @brief Helper method to find the extension in the loaded extension registry by the given filename.
                     *  @param filename The filename of the extension to search for.
                     *  @return A pointer to the extension symbol table.
                     */
                    ExtensionSymbols* findExtension(const Support::String& filename);

                // Protected Methods
                protected:
                    //! Parameter-less constructor.
                    SExtensionLoader(void);

                    //! Standard destructor.
                    ~SExtensionLoader(void);
            };
        }
    }
}
#endif // _INCLUDE_CORE_SEXTENSIONLOADER_HPP_
