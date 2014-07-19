/**
 *  @file CommandLineParser.cpp
 *  @brief Include file defining the CommandLineParser class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <support/CommandLineParser.h>

namespace Kiaro
{
    namespace Support
    {
        CommandLineParser::CommandLineParser(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[])
        {
           	size_t current_flag_hash = 0;
			const Kiaro::Common::C8 *current_flag_name = 0x00;

			static const boost::regex flag_expression("-.+", boost::regex::extended);

            for (Kiaro::Common::U32 i = 1; i < argc; i++)
				if (boost::regex_match(argv[i], flag_expression))
				{
					current_flag_name = argv[i];
					current_flag_hash = Kiaro::Common::string_hash(current_flag_name);

					this->flag_argument_count.insert(std::make_pair(current_flag_hash, 0));
					this->flags.insert(std::make_pair(current_flag_hash, boost::array<Kiaro::Common::C8 *, MAXIMUM_COMMANDLINE_ARGUMENTS>()));
				}
				else if (current_flag_name != 0x00 && this->flag_argument_count[Kiaro::Common::string_hash((Kiaro::Common::C8 *)current_flag_name)] < MAXIMUM_COMMANDLINE_ARGUMENTS)
				{
					this->flags[current_flag_hash][this->flag_argument_count[current_flag_hash]] = argv[i];
					this->flag_argument_count[current_flag_hash]++;
				}
        }

        CommandLineParser::~CommandLineParser(void) { }

        bool CommandLineParser::HasFlag(const Kiaro::Common::C8 *flag)
        {
			size_t flag_hash = Kiaro::Common::string_hash(flag);

			try
			{
				// NOTE (Robert MacGregor#1): Testing the existence of our flag by purposely raising an exception
				this->flags.at(flag_hash);
				return true;
			}
			catch (const std::out_of_range &e)
			{
				return false;
			}

			return false;
        }

        Kiaro::Common::C8 *CommandLineParser::GetFlagArgument(Kiaro::Common::C8 *target_flag, Kiaro::Common::U8 target_argument)
        {
			size_t flag_hash = Kiaro::Common::string_hash(target_flag);

			try
			{
				if (target_argument >= this->flag_argument_count[flag_hash])
					return 0x00;

				return this->flags.at(flag_hash)[target_argument];
			}
			catch (const std::out_of_range &e) { }

			return 0x00;
        }

        Kiaro::Common::U32 CommandLineParser::GetFlagArgumentCount(Kiaro::Common::C8 *target_flag)
        {
            size_t flag_hash = Kiaro::Common::string_hash(target_flag);

            if (!this->HasFlag(target_flag))
                return 0;

			return this->flag_argument_count[flag_hash];
        }
    } // End Namespace Support
} // End Namespace Kiaro
