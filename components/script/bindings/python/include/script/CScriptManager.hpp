/**
 *  @file SRenderer.hpp
 *  @brief Include file declaring the SRenderer singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2021 Draconic Entity
 */

#ifndef _INCLUDE_SCRIPT_CSCRIPTMANAGER_HPP_
#define _INCLUDE_SCRIPT_CSCRIPTMANAGER_HPP_

#include <script/IScriptManager.hpp>

namespace Kiaro
{
    namespace Script
    {
        class CScriptManager : public IScriptManager
        {
            public:
                CScriptManager(void);

                virtual bool loadMainScript(void);
        };
    } // End Namespace Script
} // End NameSpace Kiaro
#endif // _INCLUDE_SCRIPT_CSCRIPTMANAGER_HPP_
