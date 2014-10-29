/**
 *  @file Tests.h
 *  @brief Engine testing framework.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#if ENGINE_TESTS>0

#ifndef _INCLUDE_KIARO_TESTS_H_
#define _INCLUDE_KIARO_TESTS_H_

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CommandLineParser;
    }

    namespace Tests
    {
        typedef Kiaro::Common::U32 (*engineTestPtr)(Kiaro::Support::CommandLineParser *parser);

        typedef struct
        {
            Kiaro::Common::C8 *FullName;
            Kiaro::Common::C8 *FlagName;
            engineTestPtr Entry;
        } EngineTestInformation;

        void PerformTests(Kiaro::Support::CommandLineParser *parser);
    } // End NameSpace Tests
} // End NameSpace Kiaro

#endif // _INCLUDE_KIARO_TESTS_H_
#endif // ENGINE_TESTS
