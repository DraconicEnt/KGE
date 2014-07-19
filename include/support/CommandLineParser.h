/**
 *  @file CommandLineParser.h
 *  @brief Include file defining the Kiaro::Suport::CommandLineParser class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
#define _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_

#include <EngineCommon.h>
#include <EngineConfig.h>
#include "Array.hpp"
#include "String.h"

#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/regex.hpp>

namespace Kiaro
{
    namespace Support
    {
        /** Class that supplies a robust command line parser.
         *
         *  The CommandLineParser class parses console input where any arguments prepended with
         *  a dash is considered a flag while the rest is considered arguments to the flags.
         *
         *  <b>Ex:</b> ./kiaro64_d -Game base -Server 98.238.10.100 -Port 11595
         */
        class CommandLineParser
        {
            public:
            /**
             *  @brief Constructor accepting the argument count and an argument array.
             *  @param argc A Kiaro::s32 representing the total number of arguments in our array.
             *  @param argv An array of Kiaro::c8 representing the input arguments to parse.
             */
            CommandLineParser(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[]);

            //! Standard Destructor
            ~CommandLineParser(void);

            /**
             *  @brief Returns whether or not the CommandLineParser has a given flag.
             *  @param flag A Kiaro::String representing the flag name to test for.
             *  @return A boolean representing whether or not the CommandLineParser had the flag.
             */
            bool HasFlag(const Kiaro::Common::C8 *);

            Kiaro::Common::C8 *GetFlagArgument(Kiaro::Common::C8 *target_flag, Kiaro::Common::U8 target_argument);

            Kiaro::Common::U32 GetFlagArgumentCount(Kiaro::Common::C8 *target_flag);

            private:
			boost::unordered_map<size_t, boost::array<Kiaro::Common::C8 *, MAXIMUM_COMMANDLINE_ARGUMENTS> > flags;
            boost::unordered_map<size_t, Kiaro::Common::U32> flag_argument_count;
        };
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_COMMANDLINEPARSER_H_
