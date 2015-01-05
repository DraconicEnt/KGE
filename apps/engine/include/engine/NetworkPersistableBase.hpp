/**
 *  @file NetworkPersistableBase.hpp
 *  @brief Include file defining the NetworkPersistableBase base class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_NETWORKPERSISTABLEBASE_HPP_
#define _INCLUDE_KIARO_ENGINE_NETWORKPERSISTABLEBASE_HPP_

#include <unordered_map>       // std::unordered_map
#include <unordered_set>       // std::unordered_set
#include <typeinfo>
#include <type_traits> // std::is_pointer
#include <tuple>   // std::tuple
#include <typeindex> // std::type_index
#include <exception>

#include <engine/Common.hpp>
#include <support/BitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        class BitStream;
    } // End NameSpace Support

    namespace Engine
    {
        /**
         *  @brief A base class representing an object that can be serialized to
         *  and from a Kiaro::Support::BitStream.
         */
       class NetworkPersistableBase : public Kiaro::Support::SerializableObjectBase
       {
            // Public Methods
            public:
                //! Standard Constructor
                NetworkPersistableBase(void) : mFinalizedNetworkedProperties(false) { }

                /**
                void finalizeNetworkedProperties(void)
                {
                    // New map with our bit indexes
                    std::map<size_t, std::tuple<void*, size_t, size_t> > replacementMap;

                    Kiaro::Common::U32 currentBitIdentifier = 0;
                    for (std::map<size_t, std::tuple<void*, size_t, size_t> >::iterator it = mNetworkedProperties.begin(); it != mNetworkedProperties.end(); it++)
                    {
                        std::tuple<void*, size_t, size_t> currentPropertyInformation = (*it).second;
                        replacementMap[(*it).first] = std::make_tuple(std::get<0>(currentPropertyInformation), std::get<1>(currentPropertyInformation), currentBitIdentifier);

                        currentBitIdentifier++;
                    }

                    mFinalizedNetworkedProperties = true;
                    mNetworkedProperties = replacementMap;
                }
                */

                template <typename propertyType>
                void addNetworkedProperty(const std::string &name, propertyType &propertyValue)
                {
                    // Don't want to network pointer values
                    static_assert(!std::is_pointer<propertyType>::value, "SerializableObjectBase: Cannot network pointer values!");

                    // TODO (Robert MacGregor#9): Verify that the hash code calculated from typeid() is unique to each type
                    mNetworkedProperties[Kiaro::Common::string_hash(name)] = std::make_tuple(&propertyValue, typeid(propertyValue).hash_code(), 0);
                }

                template <typename propertyType>
                void setNetworkedPropertyValue(const std::string &name, const propertyType &newPropertyValue)
                {
                    static_assert(!std::is_pointer<propertyType>::value, "SerializableObjectBase: Cannot network pointer values!");

                    size_t mapIndex = Kiaro::Common::string_hash(name);
                    std::tuple<void*, size_t, size_t> networkedPropertyInfo = mNetworkedProperties[mapIndex];

                    // Is it the same type?
                    if (std::get<1>(networkedPropertyInfo) != typeid(newPropertyValue).hash_code())
                        throw std::logic_error("SerializableObjectBase: Networked property type mismatch!");

                    // Assign it
                    propertyType &oldPropertyValue = *((propertyType*)std::get<0>(networkedPropertyInfo));
                    oldPropertyValue = newPropertyValue;

                    // Add to the dirty properties
                    if (mDirtyNetworkedProperties.count(mapIndex) == 0)
                        mDirtyNetworkedProperties.insert(mDirtyNetworkedProperties.end(), mapIndex);
                }

                template <typename propertyType>
                propertyType getNetworkedPropertyValue(const std::string &name)
                {
                    static_assert(!std::is_pointer<propertyType>::value, "SerializableObjectBase: Cannot network pointer values!");

                    std::tuple<void*, size_t, size_t> networkedPropertyInfo = mNetworkedProperties[Kiaro::Common::string_hash(name)];

                    // Is it the same type?
                    if (std::get<1>(networkedPropertyInfo) != typeid(propertyType).hash_code())
                        throw std::logic_error("SerializableObjectBase: Networked property type mismatch!");

                    propertyType &returnValue = *((propertyType*)std::get<0>(networkedPropertyInfo));
                    return returnValue;
                }

            // Public Members
            public:
                //! A set of all modified network properties
                std::unordered_set<size_t> mDirtyNetworkedProperties;

            // Private Members
            private:
                bool mFinalizedNetworkedProperties;

                //! A map of string hashes (networked property names) to an std::pair representing the location & typeid hash
                std::unordered_map<size_t, std::tuple<void*, size_t, size_t> > mNetworkedProperties;
       };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_NETWORKPERSISTABLEBASE_HPP_
