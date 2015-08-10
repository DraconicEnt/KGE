/**
 *  @file INetworkPersistable.hpp
 *  @brief Include file defining the NetworkPersistableBase base class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_NET_INETWORKPERSISTABLE_HPP_
#define _INCLUDE_NET_INETWORKPERSISTABLE_HPP_

#include <unordered_map>       // std::unordered_map
#include <unordered_set>       // std::unordered_set
#include <typeinfo>
#include <type_traits> // std::is_pointer
#include <tuple>   // std::tuple
#include <typeindex> // std::type_index
#include <exception>

#include <support/UnorderedMap.hpp>
#include <support/Tuple.hpp>
#include <support/UnorderedSet.hpp>
#include <support/String.hpp>

#include <core/common.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Core
    {
        //! An enumeration containing the entity identifiers for networked objects.
        enum NETWORKED_ENTITY_NAME
        {
            //! NULL Entity, shouldn't occur.
            NETWORKED_ENTITY_NULL = 0,
            //! Any component.
            NETWORKED_ENTITY_COMPONENT = 1,
            //! A BehaviorModel.
            NETWORKED_ENTITY_BEHAVIORMODEL = 2,
        }; // End Enum NETWORKED_ENTITY_NAME

        /**
         *  @brief A base class representing an object that can be serialized to
         *  and from a Kiaro::Support::BitStream.
         */
       class INetworkPersistable : public Kiaro::Support::ISerializable
       {
            // Public Methods
            public:
                //! Standard Constructor
                INetworkPersistable(void) : mFinalizedNetworkedProperties(false) { }

                template <typename propertyType>
                void addNetworkedProperty(const Support::String& name, propertyType &propertyValue)
                {
                    // Don't want to network pointer values
                    static_assert(!std::is_pointer<propertyType>::value, "INetworkPersistable: Cannot network pointer values!");

                    // TODO (Robert MacGregor#9): Verify that the hash code calculated from typeid() is unique to each type
                    mNetworkedProperties[Kiaro::Common::string_hash(name)] = std::make_tuple(&propertyValue, typeid(propertyValue).hash_code(), 0);
                }

                template <typename propertyType>
                void setNetworkedPropertyValue(const Support::String& name, const propertyType& newPropertyValue)
                {
                    static_assert(!std::is_pointer<propertyType>::value, "INetworkPersistable: Cannot network pointer values!");

                    size_t mapIndex = Common::string_hash(name);
                    Support::Tuple<void*, size_t, size_t> networkedPropertyInfo = mNetworkedProperties[mapIndex];

                    // Is it the same type?
                    if (std::get<1>(networkedPropertyInfo) != typeid(newPropertyValue).hash_code())
                        throw std::logic_error("INetworkPersistable: Networked property type mismatch!");

                    // Assign it
                    propertyType& oldPropertyValue = *((propertyType*)std::get<0>(networkedPropertyInfo));
                    oldPropertyValue = newPropertyValue;

                    // Add to the dirty properties
                    if (mDirtyNetworkedProperties.count(mapIndex) == 0)
                        mDirtyNetworkedProperties.insert(mDirtyNetworkedProperties.end(), mapIndex);
                }

                template <typename propertyType>
                propertyType getNetworkedPropertyValue(const Support::String& name)
                {
                    static_assert(!std::is_pointer<propertyType>::value, "INetworkPersistable: Cannot network pointer values!");

                    std::tuple<void*, size_t, size_t> networkedPropertyInfo = mNetworkedProperties[Common::string_hash(name)];

                    // Is it the same type?
                    if (std::get<1>(networkedPropertyInfo) != typeid(propertyType).hash_code())
                        throw std::logic_error("INetworkPersistable: Networked property type mismatch!");

                    propertyType& returnValue = *((propertyType*)std::get<0>(networkedPropertyInfo));
                    return returnValue;
                }

                virtual void packData(Support::CBitStream& out)
                {

                }

                virtual void unpackData(Support::CBitStream& in)
                {

                }

            // Public Members
            public:
                //! A set of all modified network properties
                Support::UnorderedSet<size_t> mDirtyNetworkedProperties;

            // Private Members
            private:
                bool mFinalizedNetworkedProperties;

                //! A map of string hashes (networked property names) to an std::pair representing the location & typeid hash
                Support::UnorderedMap<size_t, Support::Tuple<void*, size_t, size_t>> mNetworkedProperties;
       };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_NET_INETWORKPERSISTABLE_HPP_
