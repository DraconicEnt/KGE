/**
 *  @brief IEngineObject.hpp
 */ 

#ifndef _INCLUDE_GAME_IENGINEOBJECT_HPP_
#define _INCLUDE_GAME_IENGINEOBJECT_HPP_

#include <support/common.hpp>

#include <support/String.hpp>

namespace Kiaro
{
    namespace Game
    {
        class IEngineObject
        {
            // Public Members
            public:
                const Common::U32 mID;
                
            // Private Members
            private:
                Support::String mName;
                
            // Public Methods
            public:
                IEngineObject(void);
                virtual ~IEngineObject(void);
                
                const Support::String& getName(void);
                
                void setName(const Support::String& name);
        };
    }
}

#endif // _INCLUDE_GAME_IENGINEOBJECT_HPP_
