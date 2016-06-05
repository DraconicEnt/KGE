/**
 */

#ifndef _INCLUDE_SUPPORT_TYPERESOLVING_HPP_
#define _INCLUDE_SUPPORT_TYPERESOLVING_HPP_

#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        //! Emumeration used to identify property types.
        enum PROPERTY_TYPE
        {
            //! Unknown type.
            PROPERTY_UNKNOWN,
            //! A Common::F32
            PROPERTY_F32,
            //! A Common::F64
            PROPERTY_F64,
            //! A Common::U32
            PROPERTY_U32,
            //! A Common::U64
            PROPERTY_U64,
            //! A Common::U8
            PROPERTY_U8,

            PROPERTY_BOOL,

            PROPERTY_STRING,

            PROPERTY_U16,

            PROPERTY_DIMENSION,
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is the base declaration that resolves to PROPERTY_UNKNOWN which is an error state.
         */
        template <typename propertyType>
        struct TypeIDResolver
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_UNKNOWN;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::F32 type.
         */
        template <>
        struct TypeIDResolver<Common::F32>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_F32;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::F64 type.
         */
        template <>
        struct TypeIDResolver<Common::F64>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_F64;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U32 type.
         */
        template <>
        struct TypeIDResolver<Common::U32>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_U32;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U64 type.
         */
        template <>
        struct TypeIDResolver<Common::U64>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_U64;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U8 type.
         */
        template <>
        struct TypeIDResolver<Common::U8>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_U8;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the bool type.
         */
        template<>
        struct TypeIDResolver<bool>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_BOOL;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Support::String type.
         */
        template<>
        struct TypeIDResolver<Support::String>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_STRING;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U16 type.
         */
        template<>
        struct TypeIDResolver<Common::U16>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_U16;
        };

        /**
         *  @brief A compile-time resolver for converting a type name to its respective PROPERTY_TYPE value.
         *  This is an explicit declaration for the Common::U16 type.
         */
        template<>
        struct TypeIDResolver<irr::core::dimension2d<Common::U32>>
        {
            static const CONSTEXPR PROPERTY_TYPE value = PROPERTY_DIMENSION;
        };
    }
}
#endif // _INCLUDE_SUPPORT_TYPERESOLVING_HPP_
