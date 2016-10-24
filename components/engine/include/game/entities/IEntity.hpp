/**
 *  @file IEntity.hpp
 *  @brief Include file declaring the IEntity interface class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_

#include <set>
#include <stdexcept>

#include <ces/IComponent.hpp>

#include <support/common.hpp>

#include <game/IEngineObject.hpp>
#include <core/SEngineInstance.hpp>
#include <filesystem/CFileReader.hpp>
#include <net/INetworkPersistable.hpp>

#include <game/entities/types.hpp>

#include <support/Set.hpp>
#include <support/UnorderedSet.hpp>
#include <support/String.hpp>
#include <support/SSettingsRegistry.hpp>

#include <core/config.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            namespace Entities
            {
                typedef Common::U32 TypeMask;
                typedef Common::U32 EntityHintMask;

                /**
                 *  @brief Enumerations representing various bit flags that may be set on an entity's flag set.
                 */
                enum FLAG_NAME
                {
                    //! A flag that tells the engine that this object needs automated net updating.
                    FLAG_UPDATING = 1,
                    //! A flag that tells the engine that this object needs update pulses.
                    FLAG_THINKING = 2,
                    //! A flag that tells the engine that this object is not scoped. This means it's networked to everyone at all times.
                    FLAG_SCOPING = 4,
                    //! A flag that tells the engine that this object very rarely updates within any context. This essentially disables differential networking.
                    FLAG_STATIC = 8,
                };

                /**
                 *  @brief The IEntity interface class is the eventual parent class of all entities that can instantiated within the engine. It allows the
                 *  type identifiers to be generated and tracked dynamically at the whim of runtime additions and removals, effectively allowing entire
                 *  types to be loaded and unloaded at will via the extension system.
                 */
                class IEntity : public Net::INetworkPersistable, public Game::IEngineObject
                {
                    // Public Members
                    public:
                        //! All flags in use by this entity.
                        const Common::U8 mFlags;

                        //! All scene nodes associated with this entity.
                        Support::UnorderedSet<irr::scene::ISceneNode*> mSceneNodes;

                    // Protected Members
                    protected:
                        //! The entity type.
                        ENTITY_TYPE mType;

                        //! The network ID of this entity.
                        Common::U32 mNetID;

                    // Public Methods
                    public:
                        /**
                         *  @brief Constructor accepting a Kiaro::Common::U32.
                         *  @param typeMask A Kiaro::Common::U32 representing the type of this
                         *  object.
                         */
                        IEntity(const ENTITY_TYPE& type, const EntityHintMask& hintMask = 0);

                        //! Standard destructor.
                        ~IEntity(void);

                        /**
                         *  @brief A struct containing unique static values going by shared names that differ based on the actual class type.
                         *  This is used to track otherwise hard coded data about the various entities that exist within the engine.
                         */
                        template <typename childName>
                        struct SharedStatics
                        {
                            //! The entity type identifier for this entity.
                            static Common::S32 sEntityTypeID;

                            #if defined(ENGINE_ENTITY_ARENA_ALLOCATIONS)
                                //! The total number of entities in our ring.
                                static size_t sEntityCount;

                                //! The total number of entities we can store in our ring.
                                static size_t sArenaSize;

                                //! The entity ring to use when ring allocation is enabled.
                                static childName* sEntityArena;

                                //! A stack of freed indices to use in the sEntityRing.
                                static Support::Stack<size_t> sFreedIndices;
                            #elif defined(ENGINE_ENTITY_TRACKER)
                                //! All entities of this type when the entity tracker is enabled for the non-ring allocator.
                                static Support::UnorderedSet<childName*> sEntities;
                            #endif
                        };

                        template <typename entityName>
                        static IEntity* constructNetworkEntity(Support::CBitStream& payload)
                        {
                            IEntity* result = new entityName(payload);
                            return result;
                        }

                        template <typename entityType>
                        void packBaseData(Support::CBitStream& out) const
                        {
                            static Common::U32 sLastPacketID = 0;
                            out << IEntity::SharedStatics<entityType>::sEntityTypeID << (sLastPacketID++);
                        }

                        /**
                         *  @brief Gets the type mask of this object.
                         *  @return A Kiaro::Common::U32 representing the typemask.
                         */
                        const Common::U32 getTypeMask(void) const;

                        /**
                         *  @brief Returns the network ID of this entity.
                         *  @return The network ID of this entity.
                         */
                        Common::U32 getNetID(void) const;

                        /**
                         *  @brief Sets the network identifier of this entity.
                         *  @param identifier The identifier to use for this entity.
                         */
                        void setNetID(const Common::U32 identifier);

                        /**
                         *  @brief Registers the entity with the simulation in the game world.
                         *  @note For clientside entities to be simulated, these should generally be within the client scope before
                         *  being networked at all.
                         */
                        virtual void registerEntity(void) = 0;

                        /**
                         *  @brief Pushes an update time pulse to the entity.
                         *  @param deltaTimeSeconds The time in seconds since the last update.
                         */
                        virtual void update(const Common::F32 deltaTimeSeconds) = 0;

                        /**
                         *  @brief Unpacks entity data from the CBitStream.
                         *  @param in The input CBitStream to unpack from.
                         */
                        virtual void unpack(Support::CBitStream& in);

                        /**
                         *  @brief Returns the type ID of the entity.
                         *  @param The type ID of this entity.
                         */
                        Common::U32 getTypeID(void) const;

                        /**
                         *  @brief Destroys the entity.
                         *  @warning This MUST be used over deleting entities directly using the inbuilt delete operator. This is because if the engine
                         *  is built with using the arena allocator, then the usual delete semantics will not work properly.
                         */
                        template <typename className>
                        void destroy(void)
                        {
                            #ifndef ENGINE_ENTITY_ARENA_ALLOCATIONS
                                delete this;
                            #else
                                const size_t freedIndex = reinterpret_cast<size_t>(this - SharedStatics<className>::sEntityArena) / sizeof(className);
                                SharedStatics<className>::sFreedIndices.push(freedIndex);
                                memset(&SharedStatics<className>::sEntityArena[freedIndex], 0x00, sizeof(className));

                                ::operator delete(this);
                            #endif
                        }
                };

                template <typename childName>
                typename Common::S32 IEntity::SharedStatics<childName>::sEntityTypeID = -1;

                #if defined(ENGINE_ENTITY_ARENA_ALLOCATIONS)
                template <typename childName>
                size_t IEntity::SharedStatics<childName>::sEntityCount = 0;

                template <typename childName>
                size_t IEntity::SharedStatics<childName>::sArenaSize = 0;

                template <typename childName>
                childName* IEntity::SharedStatics<childName>::sEntityArena = nullptr;

                template <typename childName>
                Support::Stack<size_t> IEntity::SharedStatics<childName>::sFreedIndices;
                #endif
            } // End Namespace Entities
        } // End Namespace Game
    }
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
