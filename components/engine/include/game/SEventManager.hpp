/**
 *  @file EventManagerSingleton.hpp
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_SUPPORT_SEVENTMANAGER_HPP_
#define _INCLUDE_KIARO_SUPPORT_SEVENTMANAGER_HPP_

#include <easydelegate/easydelegate.hpp>

#include <support/ISingleton.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CClient;
    } // End NameSpace Network

    namespace Engine
    {
        namespace Core
        {
            class SEventManager : public Support::ISingleton<SEventManager>
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
                    void clearEvents(void);

                // Protected Methods
                protected:
                    //! Parameter-less constructor.
                    SEventManager(void);
                    //! Standard destructor.
                    ~SEventManager(void);
            }; // End Class EventManager
        } // End NameSpace Support
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_EVENTMANAGER_HPP_
