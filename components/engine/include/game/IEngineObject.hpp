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
        /**
         *  @brief IEngineObject is the most basic class of all entities in the engine.
         *  These need not be networked entities or even things that exist in the game
         *  world as a single cohesive object.
         */
        class IEngineObject
        {
                // Public Members
            public:
                //! The ID of this object.
                const Common::U32 mID;

                // Private Members
            private:
                //! The name of this object.
                Support::String mName;

                // Public Methods
            public:
                //! Parameter-less constructor.
                IEngineObject(void);

                //! Standard virtual destructor.
                virtual ~IEngineObject(void);

                /**
                 *  @brief Returns the name of this object.
                 *  @return The name of this object.
                 */
                const Support::String& getName(void);

                /**
                 *  @brief Sets the name of this object.
                 *  @param name The new name to use.
                 */
                void setName(const Support::String& name);

                template <class objectType, typename... parameters>
                static objectType* construct(parameters... params)
                {
                    return new objectType(params...);
                }
        };
    }
}

#endif // _INCLUDE_GAME_IENGINEOBJECT_HPP_
