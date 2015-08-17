/**
 *  @file messages.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_NET_MESSAGES_HPP_
#define _INCLUDE_KIARO_NET_MESSAGES_HPP_

#include <net/messages/Disconnect.hpp>
#include <net/messages/HandShake.hpp>
#include <net/messages/Scope.hpp>
#include <net/messages/SimCommit.hpp>
#include <net/messages/ExecuteRPC.hpp>

namespace Net
{
    //! Namespace containing all the message types that can be transmitted from server to client and vice versa.
    namespace Messages
    {

    } // End NameSpace Messages
} // End NameSpace Net

#endif // _INCLUDE_KIARO_NET_MESSAGES_HPP_
