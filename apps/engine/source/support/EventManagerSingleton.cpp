/**
 *  @file EventManagerSingleton.cpp
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <support/EventManagerSingleton.hpp>

namespace Kiaro
{
    namespace Support
    {
        static EventManagerSingleton *sSingletonInstance = NULL;

        EventManagerSingleton *EventManagerSingleton::get(void)
        {
            if (!sSingletonInstance)
                sSingletonInstance = new EventManagerSingleton;

            return sSingletonInstance;
        }

        void EventManagerSingleton::destroy(void)
        {
            if (sSingletonInstance)
            {
                delete sSingletonInstance;
                sSingletonInstance = NULL;
            }
        }

        void EventManagerSingleton::clearEvents(void)
        {

        }

        EventManagerSingleton::EventManagerSingleton(void)
        {

        }

        EventManagerSingleton::~EventManagerSingleton(void)
        {

        }
    } // End NameSpace Support
} // End nameSpace Kiaro

