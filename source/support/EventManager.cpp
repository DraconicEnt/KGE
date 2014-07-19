/**
 *  @file EventManager.cpp
 *  @brief Include file defining the Kiaro::Support::BitStream class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */


#include <support/EventManager.h>

namespace Kiaro
{
    namespace Support
    {
        EventManager *Singleton_Instance = 0x00;

        EventManager *EventManager::Get(void)
        {
            if (!Singleton_Instance)
                Singleton_Instance = new EventManager;

            return Singleton_Instance;
        }

        void EventManager::Destroy(void)
        {
            if (Singleton_Instance)
            {
                delete Singleton_Instance;
                Singleton_Instance = 0x00;
            }
        }

        void EventManager::BindEvent(Kiaro::Common::C8 *name, eventDelegatePtr delegate)
        {
            boost::hash<std::string> name_hasher;
            size_t name_hash = name_hasher(name);

            try
			{
				this->EventMapping.at(name_hash).Append(delegate);
				return;
			}
			catch (const std::out_of_range &e)
			{
                this->EventMapping[name_hash] = Kiaro::Support::Array<eventDelegatePtr>();
			}

			this->EventMapping.at(name_hash).Append(delegate);
        }

        void EventManager::EmitEvent(Kiaro::Common::C8 *name, Kiaro::Common::U32 argument_count, ...)
        {
            boost::hash<std::string> name_hasher;
            size_t name_hash = name_hasher(name);

            try
			{
				Kiaro::Support::Array<eventDelegatePtr> &delegate_list = this->EventMapping.at(name_hash);

                va_list argument_list;
                va_start(argument_list, argument_count);

                va_arg(argument_list, int);

                va_end(argument_list);

                for (Kiaro::Common::U32 iteration = 0; iteration < delegate_list.Len(); iteration++)
                {
                   /// delegate_list[iteration](...);
                }

				return;
			}
			catch (const std::out_of_range &e) { } // NOTE (Robert MacGregor#1): No Events to emit.
        }

        EventManager::EventManager(void)
        {

        }

        EventManager::~EventManager(void)
        {

        }
    } // End NameSpace Support
} // End nameSpace Kiaro

