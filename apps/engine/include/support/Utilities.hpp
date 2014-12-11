/**
 *  @file Utilities.hpp
 *  @brief Various support functionality for the engine.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.0
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <EngineCommon.h>

namespace Kiaro
{
    namespace Support
    {
        /**
         *  @brief Performs an endian swap against an integer, assigning the result
         *  to the address of the input.
         *  @param from A Kiaro::Common::U32 to swap the endianness of.
         *  @return A Kiaro::Common::U32 representing the result.
         */
        static inline void SwapEndianU32Ref(Kiaro::Common::U32 &from)
        {
            #if ENGINE_INLINE_ASM<1
            from =	((255 & from) << 24) |			// First Byte to fourth position
                    ((65280 & from) << 8) |			// Second byte to third position
                    ((16711680 & from) >> 8) |		// Third byte to second position
                    ((4278190080 & from) >> 24);	// Fourth byte to first position
            #else
                #if defined(_MSC_VER)
                __asm {
                    push eax;
                    mov eax, from;
                    BSWAP eax;
                    mov from, eax;
                    pop eax;
                }
                #else
                asm ("movl %0, %%eax; BSWAP %%eax; movl %%eax, %1"
                    :"=r"(from)
                    :"r"(from)
                    :"%eax"
                    );
                #endif
            #endif
        }

        /**
         *  @brief Performs an endian swap against an integer, returning the
         *  result.
         *  @param from A Kiaro::Common::U32 to swap the endianness of.
         *  @return A Kiaro::Common::U32 representing the result.
         */
        static inline Kiaro::Common::U32 SwapEndianU32(const Kiaro::Common::U32 &from)
        {
            Kiaro::Common::U32 result = from;
            SwapEndianU32Ref(result);

            return result;
        }

        /**
         *  @brief Performs an endian swap against a long integer, assigning the result
         *  to the address of the input.
         *  @param from A Kiaro::Common::U64 to swap the endianness of.
         *  @return A Kiaro::Common::U64 representing the result.
         */
        static inline void SwapEndianU64Ref(Kiaro::Common::U64 &from)
        {
            #if ENGINE_INLINE_ASM<1
            from =  ((255 & from) << 56) |						// First Byte to eighth position
                    ((65280 & from) << 40) |					// Second byte to seventh position
                    ((16711680 & from) << 24) |					// Third byte to sixth position
                    ((4278190080 & from) << 8) | 				// Fourth byte to fifth position
                    ((1095216660480 & from) >> 8) | 			// Fifth byte to fourth position
                    ((280375465082880 & from) >> 24) |			// Sixth byte to third position
                    ((71776119061217280ULL & from) >> 40) |		// Seventh byte to second position
                    ((18374686479671623680ULL & from) >> 56);	// Eighth byte to first position
            #elif ENGINE_INLINE_ASM>1 && _ENGINE_64BIT>1
                #if defined(_MSC_VER)
                __asm {
                    push rax;
                    mov rax, from;
                    BSWAP rax;
                    mov from, rax;
                    pop rax;
                }
                #else
                asm ("mov %0, %%rax; BSWAP %%rax; mov %%rax, %1"
                    :"=r"(from)
                    :"r"(from)
                    :"%rax"
                    );
                #endif
            #endif
        }

        /**
         *  @brief Performs an endian swap against a long integer, returning the
         *  result.
         *  @param from A Kiaro::Common::U64 to swap the endianness of.
         *  @return A Kiaro::Common::U64 representing the result.
         */
        static inline Kiaro::Common::U64 SwapEndianU64(const Kiaro::Common::U64 &from)
        {
            Kiaro::Common::U64 result = from;
            SwapEndianU64Ref(result);

            return result;
        }

        /**
         *  @brief Performs an endian swap against a short integer, assinging the
         *  result to the address of the input.
         *  @param from A Kiaro::Common::U16 to swap the endianness of.
         */
        static inline void SwapEndianU16Ref(Kiaro::Common::U16 &from)
        {
            #if ENGINE_INLINE_ASM<1
            from =  ((255 & from) << 8) |	// First Byte to second position
                    ((65280 & from) >> 8);	// Second byte to first position
            #else
                #if defined(_MSC_VER)
                __asm {
                    push ax;
                    mov ax, from;
                    BSWAP ax;
                    mov from, ax;
                    pop ax;
                }
                #else
                asm ("movw %0, %%ax; BSWAP %%ax; movw %%ax, %1"
                    :"=r"(from)
                    :"r"(from)
                    :"%ax"
                    );
                #endif
            #endif
        }

        /**
         *  @brief Performs an endian swap against a short integer, returning the
         *  result.
         *  @param from A Kiaro::Common::U16 to swap the endianness of.
         *  @return A Kiaro::Common::U16 representing the result.
         */
        static inline Kiaro::Common::U16 SwapEndianU16(const Kiaro::Common::U16 &from)
        {
            Kiaro::Common::U16 result = from;
            SwapEndianU16Ref(result);

            return result;
        }

        /**
         *  @brief Performs an endian swap against a single precision float,
         *  assigning the result to the address of the input.
         *  @param from A Kiaro::Common::F32 to swap the endianness of.
         */
        static inline void SwapEndianF32Ref(Kiaro::Common::F32 &from)
        {
            SwapEndianU32Ref(*(Kiaro::Common::U32*)&from);
        }

        /**
         *  @brief Performs an endian swap against a single precision float,
         *  returning the result.
         *  @param from A Kiaro::Common::F32 to swap the endianness of.
         *  @return A Kiaro::Common::F32 representing the result.
         */
        static inline Kiaro::Common::F32 SwapEndianF32(const Kiaro::Common::F32 &from)
        {
            Kiaro::Common::F32 result = from;
            SwapEndianF32Ref(result);

            return result;
        }

        /**
         *  @brief Performs an endian swap operation against arbitrary data.
         *  @param from A pointer to the data to endian swap.
         *  @param size A Kiaro::Common::U32 representing the size of the input data to
         *  swap endianness.
         */
        static inline void SwapEndianArbitrary(void *from, const Kiaro::Common::U32 &size)
        {
            Kiaro::Common::U8 *array = (Kiaro::Common::U8*)from;

            // Create a copy of the array to read from
            Kiaro::Common::U8 *copy_from = (Kiaro::Common::U8*)malloc(size);
            memcpy(copy_from, from, size);

            for (Kiaro::Common::U32 i = 0; i < size; i++)
                array[i] = copy_from[size - i];

            free(copy_from);
        }

        /**
         *  @brief Assigns the absolute value of the input to the address of the input.
         *  @param in Any integer type supporting xor and increment operations to calculate the
         *  absolute value of.
         */
        template <typename type>
        static inline void S32Absolute(type &in)
        {
            in ^= -1; // Ditch the negative bit, but also kills the 1 bit in the process
            in++; // Handle -1 offset induced by the xor against -1
        }
    }
}
