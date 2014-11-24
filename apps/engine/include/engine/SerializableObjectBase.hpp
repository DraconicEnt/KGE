/**
 *  @file SerializableObjectBase.hpp
 *  @brief Include file defining the SerializableObjectBase class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_SERIALIZABLEOBJECTBASE_HPP_
#define _INCLUDE_KIARO_ENGINE_SERIALIZABLEOBJECTBASE_HPP_

#include <map>       // std::map
#include <bitset>    // std::bitset
#include <typeinfo>
#include <type_traits> // std::is_pointer
#include <tuple>   // std::tuple
#include <typeindex> // std::type_index
#include <exception>

#include <engine/Common.hpp>

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
       class SerializableObjectBase
       {
            // Public Methods
            public:
                virtual void packData(Kiaro::Support::BitStream &out) = 0;
                virtual void unpackData(Kiaro::Support::BitStream &in) = 0;

                //! Standard Constructor
                SerializableObjectBase(void) : mFinalizedNetworkedProperties(false) { }

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

                    std::tuple<void*, size_t, size_t> networkedPropertyInfo = mNetworkedProperties[Kiaro::Common::string_hash(name)];

                    // Is it the same type?
                    if (std::get<1>(networkedPropertyInfo) != typeid(newPropertyValue).hash_code())
                        throw std::logic_error("SerializableObjectBase: Networked property type mismatch!");

                    // Assign it
                    propertyType &oldPropertyValue = *((propertyType*)std::get<0>(networkedPropertyInfo));
                    oldPropertyValue = newPropertyValue;

                    // Add to the dirty properties
                    if (mDirtyNetworkedProperties.count(networkedPropertyInfo) == 0)
                    {
                        mDirtyNetworkedProperties.insert(mDirtyNetworkedProperties.end(), networkedPropertyInfo);

                        // Update the flags
                        mDirtyPropertyFlags[std::get<2>(networkedPropertyInfo)] = true;
                    }
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
                std::set<std::tuple<void*, size_t, size_t> > mDirtyNetworkedProperties;

                std::bitset<64> mDirtyPropertyFlags;

            // Private Members
            private:
                bool mFinalizedNetworkedProperties;

                //! A map of string hashes (networked property names) to an std::pair representing the location & typeid hash
                std::map<size_t, std::tuple<void*, size_t, size_t> > mNetworkedProperties;
       };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_SERIALIZABLEOBJECTBASE_HPP_
