/**
 *  @file SSettingsRegistry.hpp
 *  @brief Include file declaring the Support::SSettingsRegistry singleton type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CORE_SETTINGSREGISTRY_HPP_
#define _INCLUDE_CORE_SETTINGSREGISTRY_HPP_

#include <regex>

#include <support/common.hpp>

#include <support/String.hpp>
#include <support/UnorderedMap.hpp>
#include <support/Tuple.hpp>
#include <support/ISingleton.hpp>
#include <support/TypeResolving.hpp>

#include <support/Console.hpp>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief The SSettingsRegistry is a singleton type in which setting names are paired with an associated value,
         *  representing a central repository for operational parameters of the engine as a whole. This includes the
         *  config file saving & loading components of the engine.
         */
        class SSettingsRegistry : public Support::ISingleton<SSettingsRegistry>
        {
            // Private Members
            private:
                //! An unordered map mapping the setting name hashes to a pair representing the memory location and type.
                Support::UnorderedMap<Support::String, std::pair<void*, Support::PROPERTY_TYPE>> mStoredProperties;

            // Public Methods
            public:
                /**
                 *  @brief Initializes and sets defaults for all of the settings kept in the settings
                 *  registry.
                 *  @note This does not mean that all values stored here are the only ones that will
                 *  ever be here. It merely is a default, and you really *shouldn't* add others beyond
                 *  this point to reduce possibility of runtime errors.
                 */
                void setDefaultValues(void);

                /**
                 *  @brief Gets a stored setting value by its name, returning it if there is such a setting
                 *  desiginated by the given name.
                 *  @param name The name of the setting to attempt a read on.
                 *  @return The value of the named setting as whatever the requested type was, if the requested type
                 *  is the same as the actual type it is stored as.
                 *  @throw std::runtime_error Thrown when the requested setting does not exist or if the requested data
                 *  type is not the same as the data type the setting is actually stored as.
                 */
                template <typename storedType>
                storedType& getValue(const Support::String& name)
                {
                    if (mStoredProperties.find(name) == mStoredProperties.end())
                    {
                        Support::String exceptionText = "SSettingsRegistry: No such setting key: ";
                        exceptionText += name;
                        throw std::runtime_error(exceptionText);
                    }

                    // Is it the same type?
                    if (mStoredProperties[name].second != Support::TypeIDResolver<storedType>::value)
                        throw std::runtime_error("SSettingsRegistry: Property type mismatch in setting read!");

                    return *reinterpret_cast<storedType*>(mStoredProperties[name].first);
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
                    // Does an entry exist?
                    if (mStoredProperties.find(name) == mStoredProperties.end())
                    {
                        storedType* heapEntry = reinterpret_cast<storedType*>(malloc(sizeof(storedType)));

                        // This is a bit of a hack to mimic C++ allocation behavior.
                        if (!heapEntry)
                            throw std::bad_alloc();

                        // FIXME (Robert MacGregor#9): Values that own heap entries will create memory leaks
                        new (heapEntry) storedType(value);

                        CONSTEXPR Support::PROPERTY_TYPE typeID = Support::TypeIDResolver<storedType>::value;
                        mStoredProperties[name] = std::make_pair(heapEntry, typeID);
                        return;
                    }

                    std::pair<void*, Support::PROPERTY_TYPE>& networkedPropertyInfo = mStoredProperties[name];

                    // Is it the same type?
                    if (networkedPropertyInfo.second != Support::TypeIDResolver<storedType>::value)
                        throw std::runtime_error("SSettingsRegistry: Property type mismatch in setting write!");

                    // Assign it
                    storedType& oldPropertyValue = *reinterpret_cast<storedType*>(networkedPropertyInfo.first);
                    oldPropertyValue = value;
                }

                void dumpSettings(void);

                void setStringValue(const Support::String& name, const Support::String& value);

            // Protected Methods
            protected:
                //! Parameter-less constructor.
                SSettingsRegistry(void);

                //! Standard destructor.
                ~SSettingsRegistry(void);
        };
    } // End NameSpace Core
} // End NameSpace Kiaro
#endif // _INCLUDE_CORE_SETTINGSREGISTRY_HPP_
