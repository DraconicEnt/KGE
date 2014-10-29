/**
 *  @file Logging.cpp
 *  @brief Source code file defining the logic for the Kiaro::Logging system.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

#include <engine/Config.hpp>
#include <engine/Common.hpp>
#include <engine/Logging.hpp>

namespace Kiaro
{
    namespace Logging
    {
        static Kiaro::Common::U32 kiaro_logging_hook_count = 0;
        static KiaroLoggerHookPtr kiaro_logging_hooks[MAXIMUM_LOGGER_HOOKS];

        Kiaro::Common::U32 Initialize(void)
        {
            // TOOD: Figure out why this isn't properly clearing out the log.
            PHYSFS_File *file = PHYSFS_openWrite("LOG.txt");

            // Hopefully clear up the old file on Linux
            PHYSFS_write(file, " ", 1, 1);

            PHYSFS_close(file);
            //return KIARO_APPLICATION_ERROR_NONE;
            return 0;
        }

        Kiaro::Common::U32 Write(std::string input, LEVEL_NAME log_level, Kiaro::Common::U32 arg_count, ...)
        {
            PHYSFS_File *file = PHYSFS_openAppend("LOG.txt");

            if (log_level >= Kiaro::Logging::LEVEL_COUNT)
                log_level = Kiaro::Logging::LEVEL_ERROR;

           // input = input.Format("(%s): %s", 2, Kiaro::Logging::LEVEL_STRING[log_level], input.C_Str());

            //Kiaro::Common::C8 result_string[256];
            //va_list argp;
            //va_start(argp, arg_count);
            //vsnprintf(result_string, 256, input.c_str(), argp);
            //va_end(argp);

            Kiaro::Common::U32 call_result = KIARO_LOGGER_ERROR_NONE;
            if (!file)
            {
                call_result = KIARO_LOGGER_ERROR_WRITE;
                printf("-- ERROR WRITING TO THE LOGFILE --\n");
            }
            else
            {
                PHYSFS_write(file, input.c_str(), input.length(), 1);

                //if (input.find("\n") == std::string::npos)
                //    PHYSFS_write(file, "\n", 1, 1);

                PHYSFS_close(file);
            }

            for (Kiaro::Common::U32 i = 0; i < kiaro_logging_hook_count; i++)
                kiaro_logging_hooks[i]((Kiaro::Common::C8*)input.c_str(), log_level);

            //if (log_level <= 1)
            //    std::cout << result_string << std::endl;
            //else
            //    std::cerr << result_string << std::endl;

            return call_result;
        }

        Kiaro::Common::U32 AddHook(KiaroLoggerHookPtr hook)
        {
            if (kiaro_logging_hook_count >= MAXIMUM_LOGGER_HOOKS)
                return KIARO_LOGGER_ERROR_MEMALLOC;

            kiaro_logging_hooks[kiaro_logging_hook_count] = hook;
            kiaro_logging_hook_count++;

            //return KIARO_LOGGER_ERROR_NONE;
            return 0;
        }
    } // End NameSpace Logging
} // End NameSpace Kiaro
