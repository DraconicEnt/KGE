/**
 *  @file SObjectRegistry.hpp
 *  @brief Include file declaring the SObjectRegistry singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_SOBJECTREGISTRY_HPP_
#define _INCLUDE_GAME_SOBJECTREGISTRY_HPP_

#include <support/Stack.hpp>
#include <support/Deque.hpp>
#include <support/UnorderedMap.hpp>
#include <support/String.hpp>
#include <support/common.hpp>
#include <support/ISingleton.hpp>

#include <game/IEngineObject.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {

            /**
             *  @brief The SObjectRegistry is the central system with which object ID's and names are maintained. It allows lookups by
             *  both ID and name, essentially.
             *  @details All engine objects are persisted in this singleton. So not only are game world objects put in this singleton,
             *  you have miscellaneous objects that need to be persisted in some way and are not necessarily networked.
             */
            class SObjectRegistry : public Support::ISingleton<SObjectRegistry>
            {
                friend class Game::IEngineObject;

                // Private Members
                private:
                    //! A deque of engine objects for lookup by ID.
                    Support::Deque<Game::IEngineObject*> mObjects;

                    //! A stack of available object ID's, it is populated when an added object is later removed so that the ID is reused.
                    Support::Stack<Common::U32> mAvailableIDs;

                    //! A mapping of names to objects.
                    Support::UnorderedMap<Support::String, Game::IEngineObject*> mNameDictionary;

                // Public Methods
                public:
                    /**
                     *  @brief Adds a new object to the registry, using the identifier provided by the
                     *  object itself to insert it into the internal deque.
                     *  @param entity The object to add.
                     */
                    bool addObject(Game::IEngineObject* entity);

                    /**
                     *  @brief Removes an object from the registry, filling its spot with null in the
                     *  internal deque.
                     *  @param entity The object to remove.
                     */
                    bool removeObject(Game::IEngineObject* entity);

                    /**
                     *  @brief Removes an object from the registry by ID, filling its spot with null in
                     *  the internal deque.
                     *  @param identifier The object ID to remove.
                     */
                    bool removeObject(const Common::U32 identifier);

                    /**
                     *  @brief Gets an object by ID.
                     *  @param id The ID to lookup.
                     *  @return The object corresponding with the provided ID.
                     */
                    Game::IEngineObject* getObject(const Common::U32 id);

                    /**
                     *  @brief Gets an object by name.
                     *  @param name The name to lookup.
                     *  @return The object corresponding with the provided name.
                     */
                    Game::IEngineObject* getObject(const Support::String& name);

                    /**
                     *  @brief Clears the registry of all objects, at which point the
                     *  internal deque will be filled with null pointers.
                     */
                    void clear(void);

                // Protected Methods
                protected:
                    /**
                     *  @brief Grabs the next available object ID. If there are any previously used ID's now unused, those are
                     *  used first. Otherwise, we increment and extend the internal deque.
                     *  @return The next object identifier.
                     */
                    Common::U64 getNextObjectID(void);

                    /**
                     *  @brief Sets the name association of the input object.
                     *  @param object The object to set the name association of.
                     *  @param name The name to associate the object with.
                     *  @remarks If object is null, then the name in the internal map is erased
                     *  altogether.
                     */
                    void setNameEntry(Game::IEngineObject* object, const Support::String& name);

                // Protected Methods
                protected:
                    //! Parameter-less constructor.
                    SObjectRegistry(void);

                    //! Standard destructor.
                    ~SObjectRegistry(void);
            };
        }
    }
}
#endif // _INCLUDE_GAME_SOBJECTREGISTRY_HPP_
