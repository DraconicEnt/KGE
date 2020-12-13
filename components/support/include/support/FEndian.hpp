/**
 *  @file FEndian.hpp
 *  @brief Include file declaring various endian helper methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <support/common.hpp>

#ifndef _INCLUDE_KIARO_SUPPORT_FENDIAN_HPP_
#define _INCLUDE_KIARO_SUPPORT_FENDIAN_HPP_

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief Performs an endian swap against an integer, assigning the result
         *  to the address of the input.
         *  @param from A Common::U32 to swap the endianness of.
         *  @return A Common::U32 representing the result.
         */
        static inline void SwapEndianU32Ref(Common::U32& from)
        {
            #ifdef NO_INTRINSICS
                from =  ((255 & from) << 24) |          // First Byte to fourth position
                        ((65280 & from) << 8) |         // Second byte to third position
                        ((16711680 & from) >> 8) |      // Third byte to second position
                        ((4278190080 & from) >> 24);    // Fourth byte to first position
            #elif defined(_MSC_VER)
                from = _byteswap_ulong(from);
            #else
                from = __builtin_bswap32(from);
            #endif
        }

        /**
         *  @brief Performs an endian swap against an integer, returning the
         *  result.
         *  @param from A Common::U32 to swap the endianness of.
         *  @return A Common::U32 representing the result.
         */
        static inline Common::U32 SwapEndianU32(const Common::U32& from)
        {
            Common::U32 result = from;
            SwapEndianU32Ref(result);
            return result;
        }

        /**
         *  @brief Performs an endian swap against a long integer, assigning the result
         *  to the address of the input.
         *  @param from A Common::U64 to swap the endianness of.
         *  @return A Common::U64 representing the result.
         */
        static inline void SwapEndianU64Ref(Common::U64& from)
        {
            #ifdef NO_INTRINSICS
                from =  ((255 & from) << 56) |                      // First Byte to eighth position
                        ((65280 & from) << 40) |                    // Second byte to seventh position
                        ((16711680 & from) << 24) |                 // Third byte to sixth position
                        ((4278190080 & from) << 8) |                // Fourth byte to fifth position
                        ((1095216660480 & from) >> 8) |             // Fifth byte to fourth position
                        ((280375465082880 & from) >> 24) |          // Sixth byte to third position
                        ((71776119061217280ULL & from) >> 40) |     // Seventh byte to second position
                        ((18374686479671623680ULL & from) >> 56);   // Eighth byte to first position
            #elif defined(_MSC_VER)
                from = _byteswap_uint64(from);
            #else
                from = __builtin_bswap64(from);
            #endif
        }

        /**
         *  @brief Performs an endian swap against a long integer, returning the
         *  result.
         *  @param from A Common::U64 to swap the endianness of.
         *  @return A Common::U64 representing the result.
         */
        static inline Common::U64 SwapEndianU64(const Common::U64& from)
        {
            Common::U64 result = from;
            SwapEndianU64Ref(result);
            return result;
        }

        /**
         *  @brief Performs an endian swap against a short integer, assinging the
         *  result to the address of the input.
         *  @param from A Common::U16 to swap the endianness of.
         */
        static inline void SwapEndianU16Ref(Kiaro::Common::U16& from)
        {
            #ifdef NO_INTRINSICS
                from =  ((255 & from) << 8) |   // First Byte to second position
                        ((65280 & from) >> 8);  // Second byte to first position
            #elif defined(_MSC_VER)
                from = _byteswap_ushort(from);
            #else
                from = __builtin_bswap16(from);
            #endif
        }

        /**
         *  @brief Performs an endian swap against a short integer, returning the
         *  result.
         *  @param from A Common::U16 to swap the endianness of.
         *  @return A Common::U16 representing the result.
         */
        static inline Common::U16 SwapEndianU16(const Common::U16& from)
        {
            Common::U16 result = from;
            SwapEndianU16Ref(result);
            return result;
        }

        /**
         *  @brief Performs an endian swap against a single precision float,
         *  assigning the result to the address of the input.
         *  @param from A Common::F32 to swap the endianness of.
         */
        static inline void SwapEndianF32Ref(Common::F32& from)
        {
            SwapEndianU32Ref(*reinterpret_cast<Common::U32*>(&from));
        }

        /**
         *  @brief Performs an endian swap against a single precision float,
         *  returning the result.
         *  @param from A Kiaro::Common::F32 to swap the endianness of.
         *  @return A Common::F32 representing the result.
         */
        static inline Kiaro::Common::F32 SwapEndianF32(const Common::F32& from)
        {
            Common::F32 result = from;
            SwapEndianF32Ref(result);
            return result;
        }

        /**
         *  @brief Performs an endian swap operation against arbitrary data.
         *  @param from A pointer to the data to endian swap.
         *  @param size A Common::U32 representing the size of the input data to
         *  swap endianness.
         */
        static inline void SwapEndianArbitrary(void* from, const Common::U32& size)
        {
            Common::U8* array = reinterpret_cast<Common::U8*>(from);

            // Create a copy of the array to read from
            Common::U8* copy_from = reinterpret_cast<Common::U8*>(malloc(size));
            memcpy(copy_from, from, size);

            for (Common::U32 i = 0; i < size; i++)
                array[i] = copy_from[size - i];

            free(copy_from);
        }

        template <size_t dataSize>
        struct EndianSwapResolver
        {
            /**
             *  @brief The default endian swap resolver. This uses a slow software approach, but generally this shouldn't
             *  ever be fallen back to anyway (Except on platforms that have strange data type sizes).
             *  @param input A pointer to the primitive data to be endian swapped.
             *  @warning If this fallback is being used in the netcode, incompatibilities between platforms will be a problem.
             *  @todo Add a way to specify whether or not this fallback behavior is desirable.
             */
            static void resolve(void* input)
            {
                SwapEndianArbitrary(input, dataSize);
            }
        };

        template <>
        struct EndianSwapResolver<4>
        {
            /**
             *  @brief The endian swap resolver for primitive types that are 4 bytes in length.
             *  @param input A pointer to the primitive data to be endian swapped.
             */
            static void resolve(void* input)
            {
                SwapEndianU32Ref(*reinterpret_cast<Common::U32*>(input));
            }
        };

        template <>
        struct EndianSwapResolver<8>
        {
            /**
             *  @brief The endian swap resolver for primitive types that are 8 bytes in length.
             *  @param input A pointer to the primitive data to be endian swapped.
             */
            static void resolve(void* input)
            {
                SwapEndianU64Ref(*reinterpret_cast<Common::U64*>(input));
            }
        };

        template <>
        struct EndianSwapResolver<2>
        {
            /**
             *  @brief The endian swap resolver for primitive types that are 2 bytes in length.
             *  @param input A pointer to the primitive data to be endian swapped.
             */
            static void resolve(void* input)
            {
                SwapEndianU16Ref(*reinterpret_cast<Common::U16*>(input));
            }
        };

        template <typename inputType>
        static void SwapEndian(inputType& input)
        {
            EndianSwapResolver<sizeof(inputType)>::resolve(&input);
        }
    }
}
#endif // _INCLUDE_KIARO_SUPPORT_FENDIAN_HPP_
