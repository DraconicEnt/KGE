/**
 *  @file EntityBase.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
#define _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_

#include <set>
#include <stdexcept>

#include <ces/IComponent.hpp>

#include <support/common.hpp>

#include <core/SEngineInstance.hpp>
#include <filesystem/CFileReader.hpp>
#include <net/INetworkPersistable.hpp>

#include <game/entities/types.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            typedef Common::U32 TypeMask;
            typedef Common::U32 EntityHintMask;

            /**
             *  @brief Enumerations representing
             */
            enum HINT_NAME
            {
                //! A hint that tells the engine that this object does not need automated net updating. It may be pushing updates on its own.
                NO_UPDATING = 1,
                //! A hint that tells the engine that this object does not need update pulses.
                NO_THINKING = 2,
                //! A hint that tells the engine that this object is not scoped.
                NO_SCOPING = 3,
                //! A hint that tells the engine that this object very rarely updates within any context.
                IS_STATIC = 4,
            };

            class IEntity : public Core::INetworkPersistable
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
                    Common::U32 getTypeMask(void) const;

                    Common::U32 getNetID(void) const;

                    Common::U32 getHintMask(void) const;

                    void setNetID(const Common::U32& identifier);

                    virtual void packUpdate(Support::CBitStream& out);
                    virtual void unpackUpdate(Support::CBitStream& in);
                    virtual void packInitialization(Support::CBitStream& out) const;
                    virtual void unpackInitialization(Support::CBitStream& in);
                    virtual void instantiate(void);
                    virtual void update(const Common::F32& deltaTimeSeconds) = 0;

                    //virtual void setPosition(const Common::Vector3DF& position) = 0;

                // Protected Members
                protected:
                    ENTITY_TYPE mType;
                    Common::U32 mHintMask;
                    Common::U32 mNetID;

                    std::set<CES::IComponent*> mComponents;

                    std::string mName;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
