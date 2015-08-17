/**
 *  @file SSettingsRegistry.hpp
 */

#ifndef _INCLUDE_CORE_SETTINGSREGISTRY_HPP_
#define _INCLUDE_CORE_SETTINGSREGISTRY_HPP_

#include <allegro5/allegro.h>

#include <support/common.hpp>

#include <support/String.hpp>
#include <support/UnorderedMap.hpp>
#include <support/Tuple.hpp>

namespace Kiaro
{
    namespace Core
    {
        class SSettingsRegistry
        {
            // Private Members
            private:
                Support::UnorderedMap<size_t, std::pair<void*, size_t>> mStoredProperties;

            // Public Methods
            public:
                static SSettingsRegistry* getPointer(void);
                static void destroy(void);

                template <typename storedType>
                storedType& getValue(const Support::String& name)
                {
                    size_t mapIndex = Common::string_hash(name);

                    if (mStoredProperties.count(mapIndex) == 0)
                    {
                        Support::String exceptionText = "No such setting key: ";
                        exceptionText += name;

                        throw std::out_of_range(exceptionText);
                    }

                    return *reinterpret_cast<storedType*>(mStoredProperties[mapIndex].first);
                }

                template <typename storedType>
                void setValue(const Support::String& name, storedType value)
                {
                    size_t mapIndex = Common::string_hash(name);

                    // Does an entry exist?
                    if (mStoredProperties.count(mapIndex) == 0)
                    {
                        storedType* heapEntry = reinterpret_cast<storedType*>(malloc(sizeof(storedType)));

                        // This is a bit of a hack to mimic C++ allocation behavior.
                        if (!heapEntry)
                            throw std::bad_alloc();

                        // FIXME (Robert MacGregor#9): Values that own heap entries will create memory leaks
                        new (heapEntry) storedType(value);

                        mStoredProperties[mapIndex] = std::make_pair(heapEntry, typeid(storedType).hash_code());
                        return;
                    }

                    std::pair<void*, size_t> networkedPropertyInfo = mStoredProperties[mapIndex];

                    // Is it the same type?
                    if (networkedPropertyInfo.second != typeid(value).hash_code())
                        throw std::logic_error("SSettingsRegistry: Property type mismatch!");

                    // Assign it
                    storedType& oldPropertyValue = *((storedType*)(networkedPropertyInfo.first));
                    oldPropertyValue = value;
                }

            // Private Methods
            private:
                SSettingsRegistry(void);
                ~SSettingsRegistry(void);
        };
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_SETTINGSREGISTRY_HPP_
