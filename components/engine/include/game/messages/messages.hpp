/**
 *  @file messages.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_HPP_

#include <game/messages/types.hpp>
#include <game/messages/Disconnect.hpp>
#include <game/messages/HandShake.hpp>
#include <game/messages/Scope.hpp>
#include <game/messages/SimCommit.hpp>
#include <game/messages/ExecuteRPC.hpp>

namespace Kiaro
{
    namespace Game
    {
        //! Namespace containing all the message types that can be transmitted from server to client and vice versa.
        namespace Messages
        {

        } // End NameSpace Messages
    } // End NameSpace Net
}
#endif // _INCLUDE_KIARO_GAME_MESSAGES_HPP_
