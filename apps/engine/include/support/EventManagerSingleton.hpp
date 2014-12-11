/**
 *  @file EventManagerSingleton.hpp
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_EVENTMANAGER_H_
#define _INCLUDE_KIARO_SUPPORT_EVENTMANAGER_H_

#include <easydelegate.hpp>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Network
    {
        class IncomingClientBase;
    } // End NameSpace Network

    namespace Support
    {
        class EventManagerSingleton
        {
            // Public Methods
            public:
                static EventManagerSingleton *get(void);
                static void destroy(void);

                void clearEvents(void);

            // Public Typedefs
            public:
                typedef EasyDelegate::DelegateSet<void, Kiaro::Network::IncomingClientBase*> onClientConnectedEventType;
                typedef EasyDelegate::DelegateSet<void, Kiaro::Network::IncomingClientBase*> onClientDisconnectedEventType;

            // Public Members
            public:
                const onClientConnectedEventType mOnClientConnectedEvent;
                const onClientDisconnectedEventType mOnClientDisconnectedEvent;

            // Private Methods
            private:
                EventManagerSingleton(void);
                ~EventManagerSingleton(void);
        }; // End Class EventManager
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_EVENTMANAGER_H_
