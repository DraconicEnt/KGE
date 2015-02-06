/**
 *  @file Logging.hpp
 *  @brief Main include file for the Kiaro::Logging system.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_LOGGING_HPP_
#define _INCLUDE_KIARO_ENGINE_LOGGING_HPP_

#include <physfs.h>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Logging
    {
        //! An enumeration defining the log level of a message.
        enum LEVEL_NAME
        {
            //! Information.
            LEVEL_INFO = 0,
            //! Warning
            LEVEL_WARNING = 1,
            //! Error
            LEVEL_ERROR = 2,
            //! Fatal
            LEVEL_FATAL = 3,
            //! Debug
            LEVEL_DEBUG = 4,
            //! Used only for counting
            LEVEL_COUNT = 5,
        };

        //! Mapping of Log Level integers to string
        static const Kiaro::Common::C8 *LEVEL_STRING[] =
        {
            "INFORMATION",
            "WARNING",
            "ERROR",
            "FATAL",
            "DEBUG"
        };

        //! An enumeration defining logger errors.
        enum KIARO_LOGGER_ERROR_NAME
        {
            //! No error.
            KIARO_LOGGER_ERROR_NONE = 0,
            //! Cannot write to the target file.
            KIARO_LOGGER_ERROR_WRITE = 1,
            //! Memory allocation error.
            KIARO_LOGGER_ERROR_MEMALLOC = 2,
        };

        //! A function pointer definition that should be used when hooking into the logging system via callbacks.
        typedef void (*KiaroLoggerHookPtr)(Kiaro::Common::C8 *input, LEVEL_NAME log_level);

        /** Initializes the logger by verifying that the LOG.txt can be written to and clears out the old contents.
         *  @return
         */
        Kiaro::Common::U32 Initialize(void);

        /** Adds a hook to the Kiaro logging system.
         */
        Kiaro::Common::U32 AddHook(KiaroLoggerHookPtr hook);

        /** Writes to the file this logger is bound to.
         *
         * @note This function assumes that input is a NULL terminated string.
         */
        Kiaro::Common::U32 Write(const Kiaro::Common::String &input, LEVEL_NAME log_level = LEVEL_INFO, Kiaro::Common::U32 arg_count = 0, ...);
    } // End NameSpace Logging
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_LOGGING_HPP_
