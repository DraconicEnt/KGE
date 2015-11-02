/**
 *  @file SSettingsRegistry.hpp
 */

#ifndef _INCLUDE_CORE_SETTINGSREGISTRY_HPP_
#define _INCLUDE_CORE_SETTINGSREGISTRY_HPP_

#include <support/common.hpp>

#include <support/String.hpp>
#include <support/UnorderedMap.hpp>
#include <support/Tuple.hpp>

namespace Kiaro
{
    namespace Support
    {
        class SSettingsRegistry
        {
            // Private Members
            private:
                //! An unordered map mapping the setting name hashes to a pair representing the memory location and type.
                Support::UnorderedMap<size_t, std::pair<void*, size_t>> mStoredProperties;

            // Public Methods
            public:
                /**
                 *  @brief Returns the pointer to the SSettingsRegistry, creating and initializing it if
                 *  necessary before returning.
                 *  @return A pointer to the SSettingsRegistry instance.
                 */
                static SSettingsRegistry* getPointer(void);
                //! Destroys the singleton instance, if available.
                static void destroy(void);

                /**
                 *  @brief Initializes and sets defaults for all of the settings kept in the settings
                 *  registry.
                 *  @note This does not mean that all values stored here are the only ones that will
                 *  ever be here. It merely is a default, and you really *shouldn't* add others beyond
                 *  this point to reduce possibility of runtime errors.
                 */
                void setDefaultValues(void);

                template <typename storedType>
                storedType& getValue(const Support::String& name)
                {
                    size_t mapIndex = Common::string_hash(name);

                    if (mStoredProperties.count(mapIndex) == 0)
                    {
                        Support::String exceptionText = "SSettingsRegistry: No such setting key: ";
                        exceptionText += name;

                        throw std::out_of_range(exceptionText);
                    }

                    return *reinterpret_cast<storedType*>(mStoredProperties[mapIndex].first);
                }

                /**
                 *  @brief Sets a value in the settings registry, creating a new entry if
                 *  it does not exist.
                 *  @param name The name of the setting to modify or create.
                 *  @param value The value to assign to the given setting. Note that once the
                 *  setting type is set, it cannot be written or read as another type later as it
                 *  will throw an exception.
                 *  @throw std::bad_alloc Thrown when memory for a new settings entry could not be
                 *  allocated on the heap.
                 *  @throw std::runtime_error Thrown when there is a type mismatch between the setting
                 *  and the input value.
                 *  @warning This should generally only be used with primitive types to ensure that
                 *  memory leaks and other runtime errors cannot occur.
                 */
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

                    std::pair<void*, size_t>& networkedPropertyInfo = mStoredProperties[mapIndex];

                    // Is it the same type?
                    if (networkedPropertyInfo.second != typeid(value).hash_code())
                        throw std::runtime_error("SSettingsRegistry: Property type mismatch!");

                    // Assign it
                    storedType& oldPropertyValue = *(reinterpret_cast<storedType*>(networkedPropertyInfo.first));
                    oldPropertyValue = value;
                }

            // Private Methods
            private:
                //! Parameter-less constructor.
                SSettingsRegistry(void);
                //! Standard destructor.
                ~SSettingsRegistry(void);
        };
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_SETTINGSREGISTRY_HPP_
