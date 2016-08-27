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

namespace Kiaro
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

            class IEntity : public Net::INetworkPersistable, public Game::IEngineObject
            {
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
                     *  @brief Gets the type mask of this object.
                     *  @return A Kiaro::Common::U32 representing the typemask.
                     */
                    const Common::U32 getTypeMask(void) const;

                    const Common::U32 getNetID(void) const;

                    void setNetID(const Common::U32 identifier);

                    virtual void registerEntity(void) = 0;
                    virtual void update(const Common::F32 deltaTimeSeconds) = 0;

                    virtual void packDeltas(Support::CBitStream& out);
                    virtual void packEverything(Support::CBitStream& out) const;
                    virtual void unpack(Support::CBitStream& in);

                    const ENTITY_TYPE& getType(void) const;

                    //virtual void setPosition(const Common::Vector3DF& position) = 0;

                    // Public Members
                public:
                    const Common::U8 mFlags;

                    Support::UnorderedSet<irr::scene::ISceneNode*> mSceneNodes;

                    // Protected Members
                protected:
                    ENTITY_TYPE mType;
                    Common::U32 mNetID;

                    Support::Set<CES::IComponent*> mComponents;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
