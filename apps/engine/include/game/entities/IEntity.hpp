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

#include <core/common.hpp>

#include <core/SEngineInstance.hpp>
#include <filesystem/CFileReader.hpp>
#include <net/INetworkPersistable.hpp>

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

            class IEntity : public Core::INetworkPersistable
            {
                // Public Methods
                public:
                    /**
                     *  @brief Constructor accepting a Kiaro::Common::U32.
                     *  @param typeMask A Kiaro::Common::U32 representing the type of this
                     *  object.
                     */
                    IEntity(const Game::Entities::TypeMask &typeMask, const Game::Entities::EntityHintMask &hintMask = 0);

                    //! Standard destructor.
                    ~IEntity(void);

                    /**
                     *  @brief Gets the type mask of this object.
                     *  @return A Kiaro::Common::U32 representing the typemask.
                     */
                    Common::U32 getTypeMask(void) const;

                    Common::U32 getNetID(void) const;

                    Common::U32 getHintMask(void) const;

                    virtual void packUpdate(Support::CBitStream &out);
                    virtual void unpackUpdate(Support::CBitStream &in);
                    virtual void packInitialization(Support::CBitStream &out);
                    virtual void unpackInitialization(Support::CBitStream &in);
                    virtual void instantiate(void);
                    virtual void update(const Common::F32 &deltaTimeSeconds) = 0;

                // Protected Members
                protected:
                    const Common::U32 mTypeMask;
                    Common::U32 mHintMask;
                    Common::U32 mNetID;

                    std::set<CES::IComponent*> mComponents;

                    std::string mName;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
