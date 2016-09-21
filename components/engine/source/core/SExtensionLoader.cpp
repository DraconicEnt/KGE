/**
 */

#include <core/SExtensionLoader.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
            static SExtensionLoader* sInstance = nullptr;

            SExtensionLoader* SExtensionLoader::getPointer(void)
            {
                if (!sInstance)
                    sInstance = new SExtensionLoader();
                return sInstance;
            }

            void SExtensionLoader::destroy(void)
            {
                delete sInstance;
                sInstance = nullptr;
            }

            bool SExtensionLoader::loadExtension(const Support::String& filename)
            {
                // Use physFS to validate the path
                const Common::C8* basePath = PHYSFS_getRealDir(filename.data());

                if (!basePath)
                    return false;

                // If its already loaded
                if (this->findExtension(filename))
                    return false;

                // FIXME: Concat these paths correctly
                Support::String absolutePath = basePath;
                absolutePath += "/";
                absolutePath += filename;

                ExtensionSymbols symbols = this->internalLoadExtension(absolutePath);

                if (!symbols.mHandle)
                    return false;

                // Register into the extension map and return
                mExtensionMap[filename] = symbols;
                symbols.mInitialize();

                return true;
            }

            bool SExtensionLoader::unloadExtension(const Support::String& filename)
            {
                ExtensionSymbols* symbols = this->findExtension(filename);
                if (!symbols)
                    return false;

                this->internalUnloadExtension(symbols);
                this->deleteExtension(filename);

                return true;
            }

            void SExtensionLoader::deleteExtension(const Support::String& filename)
            {
                ExtensionSymbols* symbols = this->findExtension(filename);

                if (!symbols)
                    return;

                mExtensionMap.erase(filename);
            }

            SExtensionLoader::ExtensionSymbols* SExtensionLoader::findExtension(const Support::String& filename)
            {
                auto search = mExtensionMap.find(filename);

                if (search == mExtensionMap.end())
                    return nullptr;

                return &(*search).second;
            }

            bool SExtensionLoader::reloadExtension(const Support::String& filename)
            {
                // FIXME: Not implemented
                return true;
            }

            SExtensionLoader::SExtensionLoader(void)
            {

            }

            SExtensionLoader::~SExtensionLoader(void)
            {
                // Just unload everything; the memory will be cleaned up when this class is deleted anyway.
                for (auto current: mExtensionMap)
                {
                    ExtensionSymbols& symbols = current.second;
                    symbols.mDeinitialize();

                    this->internalUnloadExtension(&symbols);
                }
            }
        }
    }
}
