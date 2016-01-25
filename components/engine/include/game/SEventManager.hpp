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

#ifndef _INCLUDE_KIARO_SUPPORT_SEVENTMANAGER_HPP_
#define _INCLUDE_KIARO_SUPPORT_SEVENTMANAGER_HPP_

#include <easydelegate/easydelegate.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CClient;
    } // End NameSpace Network

    namespace Core
    {
        class SEventManager
        {
            // Public Typedefs
            public:
                typedef EasyDelegate::DelegateSet<void, Net::CClient*> onClientConnectedEventType;
                typedef EasyDelegate::DelegateSet<void, Net::CClient*> onClientDisconnectedEventType;

            // Public Members
            public:
                const onClientConnectedEventType mOnClientConnectedEvent;
                const onClientDisconnectedEventType mOnClientDisconnectedEvent;

            // Public Methods
            public:
                static SEventManager* get(void);
                static void destroy(void);

                void clearEvents(void);

            // Private Methods
            private:
                //! Parameter-less constructor.
                SEventManager(void);
                //! Standard destructor.
                ~SEventManager(void);
        }; // End Class EventManager
    } // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_EVENTMANAGER_HPP_