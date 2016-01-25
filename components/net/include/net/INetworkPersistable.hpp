/**
 *  @file INetworkPersistable.hpp
 *  @brief Include file defining the NetworkPersistableBase base class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_NET_INETWORKPERSISTABLE_HPP_
#define _INCLUDE_NET_INETWORKPERSISTABLE_HPP_

#include <typeinfo>
#include <type_traits> // std::is_pointer
#include <typeindex> // std::type_index
#include <exception>

#include <support/UnorderedMap.hpp>
#include <support/Tuple.hpp>
#include <support/UnorderedSet.hpp>
#include <support/String.hpp>

#include <support/common.hpp>
#include <support/CBitStream.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Net
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
         *  @brief An interface class representing an object that is serializable to and from
         *  a Support::CBitStream while also implementing property tracking semantics for differential
         *  synchrnonization across the network.
         */
        class INetworkPersistable : public Support::ISerializable
        {
            // Public Members
            public:
                //! A set of all modified network properties
                Support::UnorderedSet<size_t> mDirtyNetworkedProperties;

                //! Emumeration used to identify property types.
                enum PROPERTY_TYPE
                {
                    //! Unknown type.
                    PROPERTY_UNKNOWN = 0,
                    //! A Common::F32
                    PROPERTY_F32 = 1,
                    //! A Common::F64
                    PROPERTY_F64 = 2,
                    //! A Common::U32
                    PROPERTY_U32 = 3,
                    //! A Common::U64
                    PROPERTY_U64 = 4,
                    //! A Common::U8
                    PROPERTY_U8 = 5,
                };

                /**
                 *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
                 *  This is the base declaration that resolves to PROPERTY_UNKNOWN which is an error state.
                 */
                template <typename propertyType>
                struct TypeIDResolver { static constexpr PROPERTY_TYPE value = PROPERTY_UNKNOWN; };

            // Private Members
            private:
                //! A map of string hashes (networked property names) to an std::pair representing the location & typeid hash
                Support::UnorderedMap<size_t, std::pair<void*, PROPERTY_TYPE>> mNetworkedProperties;

            // Public Methods
            public:
                //! Parameterless Constructor.
                INetworkPersistable(void);

                /**
                 *  @brief Base template method to add networked properties to this INetworkPersistable
                 *  representing an erroneous input data type.
                 *  @param name The name of the property to use.
                 *  @param propertyValue The desired value to map to by name.
                 */
                template <typename propertyType>
                void addNetworkedProperty(const Support::String& name, propertyType& propertyValue)
                {
                    static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

                    constexpr PROPERTY_TYPE typeIdentifier = TypeIDResolver<propertyType>::value;
                    mNetworkedProperties[Support::getHashCode(name)] = std::make_pair(&propertyValue, typeIdentifier);
                }

                /**
                 *  @brief Templated method to modify the values of properties that were already added.
                 *  @details This is equivalent to directly writing to the actual data regularly, but it
                 *  takes note that the property has been modified and will be networked on the next packDeltas
                 *  call.
                 *  @param name The name of the property to modify.
                 *  @param newValue The desired value to use.
                 */
                template <typename propertyType>
                void setNetworkedPropertyValue(const Support::String& name, const propertyType& newValue)
                {
                    static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

                    size_t mapIndex = Support::getHashCode(name);
                    this->setNetworkedPropertyValue<propertyType>(mapIndex, newValue);
                }

                /**
                 *  @brief Templated method to modify the values of properties that were already added using their
                 *  hash code.
                 *  @details This is equivalent to directly writing to the actual data regularly, but it
                 *  takes note that the property has been modified and will be networked on the next packDeltas
                 *  call.
                 *  @param mapIndex The hash of the property name to modify.
                 *  @param newValue The desired value to use.
                 */
                template <typename propertyType>
                void setNetworkedPropertyValue(const size_t& mapIndex, const propertyType& newValue)
                {
                    static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

                    std::pair<void*, PROPERTY_TYPE> networkedPropertyInfo = mNetworkedProperties[mapIndex];

                    // Is it the same type?
                    if (networkedPropertyInfo.second != TypeIDResolver<propertyType>::value)
                        throw std::runtime_error("INetworkPersistable: Networked property type mismatch!");

                    // Assign it
                    propertyType& oldPropertyValue = *(reinterpret_cast<propertyType*>(networkedPropertyInfo.first));
                    oldPropertyValue = newValue;

                    // Add to the dirty properties
                    if (mDirtyNetworkedProperties.count(mapIndex) == 0)
                        mDirtyNetworkedProperties.insert(mDirtyNetworkedProperties.end(), mapIndex);
                }

                /**
                 *  @brief Base template method to get networked property values from this INetworkPersistable
                 *  representing an erroneous input data type.
                 */
                template <typename propertyType>
                const propertyType& getNetworkedPropertyValue(const Support::String& name)
                {
                    static_assert(TypeIDResolver<propertyType>::value != PROPERTY_UNKNOWN, "INetworkPersistable: Cannot network this data type!");

                    std::pair<void*, PROPERTY_TYPE> networkedPropertyInfo = mNetworkedProperties[Support::getHashCode(name)];

                    // Is it the same type?
                    if (networkedPropertyInfo.second != TypeIDResolver<propertyType>::value)
                        throw std::runtime_error("INetworkPersistable: Networked property type mismatch!");

                    propertyType& returnValue = *((propertyType*)networkedPropertyInfo.first);
                    return returnValue;
                }

                /**
                 *  @brief Virtual method to pack only the properties that have changed into the output
                 *  Support::CBitStream.
                 *  @param out A reference to the Support::CBitStream to write output to.
                 */
                virtual void packDeltas(Support::CBitStream& out);

                /**
                 *  @brief Virtual method to pack everything into the output Support::CBitStream. This
                 *  includes properties that have not changed.
                 *  @param out A reference to the Support::CBitStream to write output to.
                 */
                virtual void packEverything(Support::CBitStream& out) const;

                /**
                 *  @brief Virtual method to unpack data from the Support::CBitStream into
                 *  the appropriate property fields on this INetworkPersistable.
                 *  @param in A reference to the Support::CBitStream to read from.
                 */
                virtual void unpack(Support::CBitStream& in);

            // Private Methods
            private:
                /**
                 *  @brief Helper method used to pack arbitrary methods into the INetworkPersistable.
                 *  @param out A reference to the Support::CBitStream to write into.
                 *  @param propertyHash The hash code of the property to use.
                 *  @param property A reference to the Support::Tuple to write.
                 */
                inline void packProperty(Support::CBitStream& out, const size_t& propertyHash, const std::pair<void*, PROPERTY_TYPE>& property) const;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::F32 type.
         */
        template <>
        struct INetworkPersistable::TypeIDResolver<Common::F32> { static constexpr PROPERTY_TYPE value = INetworkPersistable::PROPERTY_F32; };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::F64 type.
         */
        template <>
        struct INetworkPersistable::TypeIDResolver<Common::F64> { static constexpr PROPERTY_TYPE value = INetworkPersistable::PROPERTY_F64; };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U32 type.
         */
        template <>
        struct INetworkPersistable::TypeIDResolver<Common::U32> { static constexpr PROPERTY_TYPE value = INetworkPersistable::PROPERTY_U32; };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U64 type.
         */
        template <>
        struct INetworkPersistable::TypeIDResolver<Common::U64> { static constexpr PROPERTY_TYPE value = INetworkPersistable::PROPERTY_U64; };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U8 type.
         */
        template <>
        struct INetworkPersistable::TypeIDResolver<Common::U8> { static constexpr PROPERTY_TYPE value = INetworkPersistable::PROPERTY_U8; };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_NET_INETWORKPERSISTABLE_HPP_
