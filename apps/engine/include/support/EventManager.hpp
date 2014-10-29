/**
 *  @file EventManager.h
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

#include <cstdarg>

#include <boost/unordered_map.hpp>

#include <support/Array.hpp>
#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        //! Expected signature for delegates to be used in the event system.
        typedef void (*eventDelegatePtr)(va_list);

        class EventManager
        {
            public:
            static EventManager *Get(void);
            static void Destroy(void);

            void BindEvent(Kiaro::Common::C8 *name, eventDelegatePtr delegate);
            void EmitEvent(Kiaro::Common::C8 *name, Kiaro::Common::U32 argument_count, ...);

            private:
            EventManager(void);
            ~EventManager(void);

            boost::unordered_map<size_t, Kiaro::Support::Array<eventDelegatePtr> > EventMapping;
        }; // End Class EventManager
    } // End NameSpace Support
} // End nameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_EVENTMANAGER_H_
