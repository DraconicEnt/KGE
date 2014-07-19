/**
 *  @file BitStream.h
 *  @brief Engine testing framework.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <EngineConfig.h>

#if ENGINE_TESTS>0

#ifndef _INCLUDE_KIARO_TESTS_BITSTREAM_H_
#define _INCLUDE_KIARO_TESTS_BITSTREAM_H_

#include <tests/Tests.h>

#include <support/BitStream.h>
#include <support/CommandLineParser.h>

#include <EngineLogging.h>

namespace Kiaro
{
    namespace Tests
    {
        enum EXIT_STATUS_NAME
        {
            EXIT_STATUS_GOOD = 0,
            EXIT_STATUS_MISMATCHED_FLOAT = 1,
            EXIT_STATUS_MISMATCHED_STREAM_SIZE = 2,
        };

        static Kiaro::Common::F32 float_list[] =
        {
            3.14159,
            1337.125,
        };

        static Kiaro::Common::U32 float_count = sizeof(float_list) / sizeof(Kiaro::Common::F32);

        inline void PackFloats(Kiaro::Support::BitStream *destination)
        {
            for (Kiaro::Common::U32 iteration = 0; iteration < float_count; iteration++)
                destination->WriteF32(float_list[iteration]);
        }

        static Kiaro::Common::U32 BitStreamTest(Kiaro::Support::CommandLineParser *parser)
        {
            Kiaro::Common::U32 exit_status = 0;

            // Test one: Write a handful of floats and read it back; using a BitStream that knows how much memory to use, therefore it shouldn't change sizes

            Kiaro::Common::U32 expected_stream_size = float_count * sizeof(Kiaro::Common::F32);

            Kiaro::Support::BitStream *float_stream = new Kiaro::Support::BitStream(expected_stream_size);
            PackFloats(float_stream);

            if (float_stream->TotalSize() != expected_stream_size)
            {
                exit_status |= EXIT_STATUS_MISMATCHED_STREAM_SIZE;
                Kiaro::Logging::Write("Mismatched stream size: %u != %u", Kiaro::Logging::LEVEL_ERROR, 2, float_stream->TotalSize(), expected_stream_size);
            }

            for (Kiaro::Common::S32 iteration = float_count - 1; iteration > -1; iteration--)
            {
                Kiaro::Common::F32 read_float = float_stream->ReadF32();
                if (read_float != float_list[iteration])
                {
                    exit_status |= EXIT_STATUS_MISMATCHED_FLOAT;
                    Kiaro::Logging::Write("Mismatched float at index %u: %f != %f", Kiaro::Logging::LEVEL_ERROR, 3, iteration, read_float, float_list[iteration]);
                }
            }

            delete float_stream;

            // Test Two: Write the same floats above except using a bad starting size
            float_stream = new Kiaro::Support::BitStream(2);
            delete float_stream;

            return exit_status;
        }
    } // End NameSpace Tests
} // End NameSpace Kiaro

#endif // _INCLUDE_KIARO_TESTS_H_
#endif // ENGINE_TESTS
