/**
 *  @file FCPUInfo.hpp
 *  @brief Include file defining standard methods of obtaining information about the CPU that is running the engine.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 *
 *  @todo Generalize this library to be architecture neutral.
 */

#include <support/String.hpp>

namespace Kiaro
{
    namespace Support
    {
        static void getCPUVendorID(Support::String& output)
        {
            // On x86, the vendor string is always twelve characters
            char result[13];
            result[12] = 0x00;

            Common::U32& outEbx = *reinterpret_cast<Common::U32*>(result);
            Common::U32& outEdx = *reinterpret_cast<Common::U32*>(&result[4]);
            Common::U32& outEcx = *reinterpret_cast<Common::U32*>(&result[8]);

            #if defined(_MSC_VER)
                __asm
                {
                    push eax;
                    push ebx;
                    push edx;
                    push ecx;

                    mov eax, 0;
                    cpuid;

                    mov outEbx, ebx;
                    mov outEdx, edx;
                    mov outEcx, ecx;

                    pop eax;
                    pop ebx;
                    pop edx;
                    pop ecx;
                }
            #else
                asm ("movl $0, %%eax; cpuid; movl %%ebx, %0; movl %%edx, %1; movl %%ecx, %2;"
                 :"=r"(outEbx), "=r"(outEdx), "=r"(outEcx)
                 :
                 : "%eax", "%ebx", "%edx", "%ecx");
            #endif

            output = result;
        }

        static void getCPUBrand(Support::String& output)
        {
            char result[16];
            result[15] = 0x00;

            Common::U32& outEax = *reinterpret_cast<Common::U32*>(&result[0]);
            Common::U32& outEbx = *reinterpret_cast<Common::U32*>(&result[4]);
            Common::U32& outEcx = *reinterpret_cast<Common::U32*>(&result[8]);
            Common::U32& outEdx = *reinterpret_cast<Common::U32*>(&result[12]);

            #if defined(_MSC_VER)
                __asm
                {
                    push eax;
                    push ebx;
                    push edx;
                    push ecx;

                    movl 0x80000000, eax;
                    cpuid;
                    cmp eax, 0x80000004;
                    je supported;

                    unsupported:
                    mov outEax, 0;
                    mov outEbx, 0;
                    mov outEcx, 0;
                    mov outEdx, 0;
                    jmp done;

                    supported:
                    mov eax, 0x80000002;
                    cpuid;
                    mov eax, outEax;
                    mov ebx, outEbx;
                    mov ecx, outEcx;
                    mov edx, outEdx;

                    pop eax;
                    pop ebx;
                    pop edx;
                    pop ecx;
                }
            #else
                asm ("movl $0x80000000, %%eax;"
                "cpuid;"
                "cmp $0x80000004, %%eax;"
                "je supported;"

                // If the functionality isn't supported, just dump null bytes and we'll produce a blank string
                "unsupported:"
                "movl $0, %0;"
                "movl $0, %1;"
                "movl $0, %2;"
                "movl $0, %3;"
                "jmp done;"

                // If the functionality is supported, copy the register values after another cpuid call
                "supported:"
                "movl $0x80000002, %%eax;"
                "cpuid;"
                "movl %%eax, %0;"
                "movl %%ebx, %1;"
                "movl %%ecx, %2;"
                "movl %%edx, %3;"

                "done: "
                 :"=r"(outEax), "=r"(outEbx), "=r"(outEcx), "=r"(outEdx)
                 :
                 : "%eax", "%ebx", "%edx", "%ecx");
            #endif

            output = result;
        }

        //! A struct representing general CPU information.
        struct CPUInfo
        {
            //! The CPU stepping.
            Common::U8 mStepping;
            //! The CPU model.
            Common::U8 mModel;
            //! The CPU family.
            Common::U8 mFamily;
            //! The CPU type.
            Common::U8 mType;
            //! The CPU extended model.
            Common::U8 mExtendedModel;
            //! The CPU extended family.
            Common::U8 mExtendedFamily;
        };

        /**
         *  @brief Obtains general CPU information, writing it the input CPUInfo struct.
         *  @param output The reference of the CPUInfo struct to write to.
         */
        static void getCPUInfo(CPUInfo& output)
        {
            Common::U32 outputBits = 0;

            #if defined(_MSC_VER)
                __asm
                {
                    push eax;
                    push ecx;
                    push edx;

                    mov eax, 1;
                    cpuid;
                    mov outputBits, eax;


                    pop eax;
                    pop ecx;
                    pop edx;
                }
            #else
                asm ("movl $1, %%eax;"
                "cpuid;"
                "movl %%eax, %0;"

                 :"=r"(outputBits)
                 :
                 : "%eax", "%ecx", "%edx");
            #endif

            // Interpret the CPU info bits
            output.mStepping = 0xFF & outputBits;
            output.mModel = 0xFF00 & outputBits;
            output.mFamily = 0xFF0000 & outputBits;

            output.mType = ((1 << 13) | (1 << 12)) & outputBits;
            output.mExtendedModel = 0xFF00000000 & outputBits;
            output.mExtendedFamily = 0xFF000000000 & outputBits;
        }

        /**
         *  @brief An enumeration representing CPU features.
         *  @note The available feature bit values here will change based upon the target microarchitecture.
         */
        enum CPUFeature_Name
        {
            // eax = 80000001h edx Feature bits
            CPUFeature_MMX = (1 << 23),
            CPUFeature_MMXEXT = (1 << 22),

            // eax = 80000001h ecx Feature bits
            CPUFeature_SSE4A = (1 << 6),

            // eax = 1 edx feature bits
            CPUFeature_SSE = (1 << 25),
            CPUFeature_SSE2 = (1 << 26),
            CPUFeature_Clfsh = (1 << 19),
            CPUFeature_HTT = (1 << 28),

            // eax = 1 ecx feature bits
            CPUFeature_SSSE3 = (1 << 9),
            CPUFeature_SSE41 = (1 << 19),
            CPUFeature_SSE42 = (1 << 20),
            CPUFeature_AVX = (1 << 28),
            CPUFeature_SSE3 = 1,
        };

        /**
         *  @brief A struct representing the CPU features available for the system.
         *  @todo Should probably optimize this into a bitmask once I have all the information here I need.
         */
        struct CPUSupportedFeatures
        {
            //! Whether or not MMX is supported.
            bool mMMX;
            //! Whether or not MMX extensions are supported.
            bool mMMXExt;
            //! Is SSE4a supported?
            bool mSSE4A;
            //! Is SSE supported?
            bool mSSE;
            //! Is SSE2 supported?
            bool mSSE2;
            //! Is SSE3 supported?
            bool mSSE3;
            //! Is SSSE3 supported?
            bool mSSSE3;
            //! Is SSE4.1 supported?
            bool mSSE41;
            //! Is SSE4.2 supported?
            bool mSSE42;
            //! Is advanced vector extensions supported?
            bool mAVX;
            //! Is the clfsh command supported?
            bool mClfsh;
            //! Is hyper threading present?
            bool mHTT;
        };

        static void getCPUSupportedFeatures(CPUSupportedFeatures& output)
        {
            Common::U32 outputEdx = 0;
            Common::U32 outputEcx = 0;

            #if defined(_MSC_VER)
                __asm
                {
                    push eax;
                    push ecx;
                    push edx;

                    mov eax, 1;
                    cpuid;
                    mov outputEdx, edx;
                    mov outputEcx, ecx;

                    pop eax;
                    pop ecx;
                    pop edx;
                }
            #else
                asm ("movl $1, %%eax;"
                "cpuid;"
                "movl %%edx, %0;"
                "movl %%ecx, %1;"

                 :"=r"(outputEdx), "=r"(outputEcx)
                 :
                 : "%eax", "%edx", "%ecx");
            #endif

            // Interpret the bits
            output.mSSE = outputEdx & CPUFeature_SSE;
            output.mSSE2 = outputEdx & CPUFeature_SSE2;
            output.mClfsh = outputEdx & CPUFeature_Clfsh;
            output.mHTT = outputEdx & CPUFeature_HTT;

            output.mSSSE3 = outputEdx & CPUFeature_SSSE3;
            output.mSSE41 = outputEdx & CPUFeature_SSE41;
            output.mSSE42 = outputEdx & CPUFeature_SSE42;
            output.mAVX = outputEdx & CPUFeature_AVX;
            output.mSSE3 = outputEdx & CPUFeature_SSE3;
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
