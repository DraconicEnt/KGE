/**
 *  @file SGameWorld.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_ENGINE_SGAMEWORLD_HPP_
#define _INCLUDE_KIARO_ENGINE_SGAMEWORLD_HPP_

#include <support/ISingleton.hpp>
#include <support/CBitStream.hpp>
#include <support/UnorderedSet.hpp>
#include <support/String.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            class IGameMode;

            namespace Entities
            {
                class IEntity;
                class CSky;
            } // End NameSpace Entities

            /**
             *  @brief The SGameWorld is the registry at which all entities will be tracked. This is utilized on both the client and server
             *  ends to track clientside and serverside entities accordingly.
             */
            class SGameWorld : public Support::ISingleton<SGameWorld>, public Support::ISerializable
            {
                friend class Entities::IEntity;

                // Private Members
                private:
                    //! A set of all entities registered to the game world for updates.
                    Support::UnorderedSet<Entities::IEntity*> mEntities;

                    //! Pointer to the sky entity.
                    Entities::CSky* mSky;

                    //! Pointer to the gamemode programming that is currently running.
                    IGameMode* mGameMode;

                // Public Members
                public:
                    //! An iterator used to iterate over all entities in the SGameWorld.
                    typedef Support::UnorderedSet<Entities::IEntity*>::iterator iterator;
                    //! A const iterator used to iterate over all entities in the SGameWorld.
                    typedef Support::UnorderedSet<Entities::IEntity*>::const_iterator const_iterator;

                // Public Methods
                public:
                    /**
                     *  @brief Registers an entity with the SGameWorld.
                     *  @param entity A pointer to the entity to register.
                     */
                    void addEntity(Entities::IEntity* entity);

                    /**
                     *  @brief Removes an entity from the SGameWorld. This does not delete it, therefore the entity must be
                     *  destructed independently of this call.
                     *  @param A pointer to the entity to remove.
                     */
                    void removeEntity(Entities::IEntity* entity);

                    /**
                     *  @brief Removes an entity from the SGameWorld. This does not delete it, therefore the entity must be
                     *  destructed independently of this call.
                     *  @param The ID of the entity to remove.
                     *  @return A pointer to the entity that was removed.
                     */
                    Entities::IEntity* removeEntity(const Common::U32 id);

                    /**
                     *  @brief Looks up an entity with the given identifier.
                     *  @param id The ID to use when looking up the entity.
                     *  @return The pointer to the entity that was looked up.
                     *  @retval nullptr No entity with the specified ID was found.
                     */
                    Entities::IEntity* getEntity(const Common::U32 id) const;

                    /**
                     *  @brief Looks up an entity with the given name.
                     *  @param name The name to use when looking up the entity.
                     *  @return The pointer to the entity that was looked up.
                     *  @retval nullptr No entity with the specified name was found.
                     */
                    Entities::IEntity* getEntity(const Support::String& name) const;

                    /**
                     *  @brief Pushes an empty to the SGameWorld, triggering all entities to be updated.
                     *  @param deltaTimeSeconds The delta time in seconds that has passed since the last call.
                     */
                    void update(const Common::F32 deltaTimeSeconds);

                    /**
                     *  @brief Clears all entities from the game world.
                     */
                    void clear(void);

                    /**
                     *  @brief Retries the sky associated with this game world.
                     *  @return A pointer to the sky entity associated with this game world.
                     */
                    Entities::CSky* getSky(void);

                    /**
                     *  @brief Returns the amount of memory required to pack the SGameWorld into a CBitStream.
                     *  @return The amount of bytes required to pack the SGameWorld into a CBitStream.
                     */
                    size_t getRequiredMemory(void) const;

                    /**
                     *  @brief Packs the entirety of the SGameWorld into a CBitStream.
                     *  @param out A reference to the CBitStream to pack into.
                     */
                    void packEverything(Support::CBitStream& out) const;

                    /**
                     *  @brief Unpacks updates to the SGameWorld from an input CBitStream.
                     *  @param in A reference to the CBitStream to unpack from.
                     */
                    void unpack(Support::CBitStream& in);

                    /**
                     *  @brief Sets a new game mode for the SGameWorld to run.
                     *  @param game A pointer to the new game mode to run. The setup method will be called once the teardown method on the
                     *  previous gamemode has been called.
                     *  @note Ensure that all game modes have their setup and teardown methods implemented correctly, otherwise undefined
                     *  behavior is likely to occur.
                     */
                    void setGameMode(IGameMode* game);

                    /**
                     *  @brief Returns the current game mode in use by the SGameWorld.
                     *  @return A pointer to the current game mmode in use by the SGameWorld.
                     */
                    IGameMode* getGameMode(void);

                    /**
                     *  @brief Returns an iterator pointing to the first entity in the SGameWorld.
                     *  @return An iterator pointing to the first entity in the SGameWorld.
                     */
                    iterator begin(void);

                    /**
                     *  @brief Returns an iterator pointing to the end of the SGameWorld.
                     *  @return An iterator pointing to the last entity in the SGameWorld.
                     */
                    const_iterator end(void);

                    /**
                     *  @brief Instantiates a new instances of the template provided class. This method only claims the memory
                     *  required to represent the requested entity at the most basic level, it will not be properly initialized
                     *  until registerEntity is called to also register it with the game world.
                     *  @param all All parameters will be automatically passed in to the requested entity type's constructor.
                     *  @return A pointer to the allocated entity memory.
                     */
                    template <typename className, typename... parameters>
                    static className* instantiate(parameters... params)
                    {
                        className* result = nullptr;

                        #ifndef ENGINE_ENTITY_ARENA_ALLOCATIONS
                            result = new className(params...);

                            #ifdef ENGINE_ENTITY_TRACKER
                                Entities::IEntity::SharedStatics<className>::sEntities.insert(Entities::IEntity::SharedStatics<className>::sEntities.end(), result);
                            #endif
                        #else
                            // Allocate the arena
                            if (!Entities::IEntity::SharedStatics<className>::sEntityArena)
                            {
                                #ifndef ENGINE_ENTITY_ARENA_ALLOCATION_SIZE
                                    Support::SSettingsRegistry* settings = Support::SSettingsRegistry::instantiate();
                                    const Common::U32 arenaAllocationSize = settings->getValue<Common::U32>("System::ArenaAllocationSize");
                                #else
                                    const Common::U32 arenaAllocationSize = ENGINE_ENTITY_ARENA_ALLOCATION_SIZE;
                                #endif

                                Entities::IEntity::SharedStatics<className>::sEntityArena = reinterpret_cast<className*>(malloc(sizeof(className) * arenaAllocationSize));
                                Entities::IEntity::SharedStatics<className>::sArenaSize = arenaAllocationSize;

                                CONSOLE_DEBUGF("Allocated arena buffer for %s", typeid(className).name());
                            }

                            // If we exceed the arena buffer size, we are screwed beyond belief since we can't simply resize and invalidate pointers by moving our buffer
                            // FIXME: Unless we allocate new arenas to "extend" our buffer with, at the cost of some efficiency
                            assert(Entities::IEntity::SharedStatics<className>::sEntityCount < Entities::IEntity::SharedStatics<className>::sArenaSize);

                            if (!Entities::IEntity::SharedStatics<className>::sFreedIndices.size())
                                result = &Entities::IEntity::SharedStatics<className>::sEntityArena[Entities::IEntity::SharedStatics<className>::sEntityCount++];
                            else
                            {
                                size_t index = Entities::IEntity::SharedStatics<className>::sFreedIndices.top();
                                result = &Entities::IEntity::SharedStatics<className>::sEntityArena[index];
                                Entities::IEntity::SharedStatics<className>::sFreedIndices.pop();
                            }

                            new (result) className(params...);
                        #endif

                        assert(result);
                        return result;
                    }

                    /**
                     *  @brief Dispatches an update to all entities of the specified type in the template.
                     *  @param deltaTimeSeconds The time in seconds since the last update.
                     *  @warning This is for internal use only to provide more efficient update routines when arena allocations are enabled.
                     */
                    template <typename className>
                    static inline void update(Common::F32 deltaTimeSeconds)
                    {
                        #ifndef ENGINE_ENTITY_ARENA_ALLOCATIONS
                            throw std::runtime_error("Cannot update individual types when the engine is compiled without ring allocations.");
                        #else
                            for (size_t iteration = 0; iteration < Entities::IEntity::SharedStatics<className>::sEntityCount; ++iteration)
                            {
                                className& entity = Entities::IEntity::SharedStatics<className>::sEntityArena[iteration];

                                // The flags should never be NULL
                                if (!entity.mFlags)
                                    continue;

                                entity.update(deltaTimeSeconds);
                            }
                        #endif
                    }

                // Protected Methods
                protected:
                    //! Standard constructor
                    SGameWorld(void);

                    //! Standard destructor
                    ~SGameWorld(void);
            };
        } // End Namespace Game
    } // End NameSpace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_SGAMEWORLD_HPP_
