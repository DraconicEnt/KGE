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

#include <ces/ComponentBase.hpp>

#include "engine/Common.hpp"

#include <engine/CoreSingleton.hpp>
#include <engine/FileReadObject.hpp>
#include <engine/NetworkPersistableBase.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            typedef Kiaro::Common::U32 TypeMask;
            typedef Kiaro::Common::U32 EntityHintMask;

            /**
             *  @brief Enumerations representing
             */
            enum ENTITYHINT_NAME
            {
                //! NULL hint.
                ENTITYHINT_NULL = 0,
                //! A hint that tells the engine that this object does not need automated net updating. It may be pushing updates on its own.
                ENTITYHINT_NONUPDATING = 1,
                //! A hint that tells the engine that this object does not need update pulses.
                ENTITYHINT_NONTHINKING = 2,
                //! A hint that tells the engine that this object very rarely updates within any context.
                ENTITYHINT_STATIC = 4,
            };

            class EntityBase : public Kiaro::Engine::NetworkPersistableBase
            {
                // Public Methods
                public:
                    /**
                     *  @brief Constructor accepting a Kiaro::Common::U32.
                     *  @param typeMask A Kiaro::Common::U32 representing the type of this
                     *  object.
                     */
                    EntityBase(const Kiaro::Game::Entities::TypeMask &typeMask, const Kiaro::Game::Entities::EntityHintMask &hintMask = 0);

                    //! Standard destructor.
                    ~EntityBase(void);

                    /**
                     *  @brief Gets the type mask of this object.
                     *  @return A Kiaro::Common::U32 representing the typemask.
                     */
                    Kiaro::Common::U32 getTypeMask(void) const;

                    Kiaro::Common::U32 getNetID(void) const;

                    Kiaro::Common::U32 getHintMask(void) const;

                    virtual void packUpdate(Kiaro::Support::BitStream &out);
                    virtual void unpackUpdate(Kiaro::Support::BitStream &in);
                    virtual void packInitialization(Kiaro::Support::BitStream &out);
                    virtual void unpackInitialization(Kiaro::Support::BitStream &in);
                    virtual void instantiate(void);
                    virtual void update(const Kiaro::Common::F32 &deltaTimeSeconds) = 0;

                // Protected Members
                protected:
                    const Kiaro::Common::U32 mTypeMask;
                    Kiaro::Common::U32 mHintMask;
                    Kiaro::Common::U32 mNetID;

                    std::set<Kiaro::CES::ComponentBase*> mComponents;

                    std::string mName;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
