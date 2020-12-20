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

#include <stdio.h>

#include <support/Console.hpp>
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


            output = "<UNKNOWN>";
        }

        static void getCPUBrand(Support::String& output)
        {
            char result[16];
            result[15] = 0x00;

            output = "<UNKNOWN>";
        }

        //! A struct representing general CPU information.
        struct CPUInfo
        {
            //! The CPU stepping.
            Common::U16 mStepping;
            //! The CPU model.
            Common::U16 mModel;
            //! The CPU family.
            Common::U16 mFamily;
            //! The CPU type.
            Common::U16 mType;
            //! The CPU extended model.
            Common::U16 mExtendedModel;
            //! The CPU extended family.
            Common::U16 mExtendedFamily;
            //! The number of CPU cores present on the machine.
            Common::U16 mCoreCount;
        };

        /**
         *  @brief Obtains general CPU information, writing it the input CPUInfo struct.
         *  @param output The reference of the CPUInfo struct to write to.
         */
        static void getCPUInfo(CPUInfo& output)
        {
            Common::U32 outputBits = 0;
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

        static Common::F32 getSystemMemoryTotal(void)
        {
            return 8000.0f;
            // On Linux we have /proc/cpuinfo to determine the number of online cores
            // FIXME: Is there any Linux kernel calls we can use specifically for this?
            /*
            FILE* handle = popen("cat /proc/meminfo | grep MemTotal | awk '{print $2}'", "r");

            if (!handle)
                return 0.0f;
            else
            {
                char buffer[32];
                fgets(buffer, 32, handle);
                buffer[31] = 0x00;

                // The data is in kB so we convert to GB
                Common::F32 result = (atof(buffer) / 1024) / 1024;
                pclose(handle);
                return result;
            }

            return 0.0f;
            */
        }

        static void getCPUSupportedFeatures(CPUSupportedFeatures& output)
        {
            Common::U32 outputEdx = 0;
            Common::U32 outputEcx = 0;
        }

        //! Static instance of CPU info.
        static CPUInfo sCPUInfo;
        //! Static instance of all supported CPU features.
        static CPUSupportedFeatures sCPUSupportedFeatures;

        static void getCPUInformation()
        {
            getCPUInfo(sCPUInfo);
            getCPUSupportedFeatures(sCPUSupportedFeatures);

            // Print CPU data to stdout
            Support::String cpuVendorID;
            getCPUVendorID(cpuVendorID);

            Support::String cpuBrand;
            getCPUBrand(cpuBrand);
            cpuBrand = cpuBrand.empty() ? "<Unable to get Brand>" : cpuBrand;

            CONSOLE_INFO("CPU Information Begin -----------------------------------------");
            CONSOLE_INFOF("CPU Vendor ID: %s", cpuVendorID.data());
            CONSOLE_INFOF("CPU Brand: %s", cpuBrand.data());
            CONSOLE_INFOF("Model: %u, Family: %u, Type: %u", sCPUInfo.mModel, sCPUInfo.mFamily, sCPUInfo.mType);
            CONSOLE_INFOF("Extended Model: %u, Extended Family: %u", sCPUInfo.mExtendedModel, sCPUInfo.mExtendedFamily);
            CONSOLE_INFOF("Stepping: %u", sCPUInfo.mStepping);
            CONSOLE_INFOF("CPU Core Count: %u", sCPUInfo.mCoreCount);
            CONSOLE_INFOF("Total System Memory: %f GB", getSystemMemoryTotal());

            // Microarchitecture specific data
            CONSOLE_INFOF("HTT: %u, Clfsh: %u", sCPUSupportedFeatures.mHTT, sCPUSupportedFeatures.mClfsh);
            CONSOLE_INFO("CPU Information End -------------------------------------------");
        }
    } // End NameSpace Support
} // End NameSpace Kiaro
