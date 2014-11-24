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

#include <stdexcept>

#include "engine/Common.hpp"

#include <engine/CoreSingleton.hpp>
#include <engine/FileReadObject.hpp>
#include <engine/SerializableObjectBase.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Entities
        {
            typedef Kiaro::Common::U32 TypeMask;

            class EntityBase : public Kiaro::Engine::SerializableObjectBase
            {
                // Public Methods
                public:
                    /**
                     *  @brief Constructor accepting a Kiaro::Common::U32.
                     *  @param typeMask A Kiaro::Common::U32 representing the type of this
                     *  object.
                     */
                    EntityBase(const Kiaro::Game::Entities::TypeMask &typeMask);

                    //! Standard destructor.
                    ~EntityBase(void);

                    void setShapeFile(const std::string &filename);

                    /**
                     *  @brief Gets the type mask of this object.
                     *  @return A Kiaro::Common::U32 representing the typemask.
                     */
                    Kiaro::Common::U32 getTypeMask(void) const;

                    Kiaro::Common::U32 getNetID(void) const;

                    virtual void packUpdate(Kiaro::Support::BitStream &out);
                    virtual void unpackUpdate(Kiaro::Support::BitStream &in);
                    virtual void packInitialization(Kiaro::Support::BitStream &out);
                    virtual void unpackInitialization(Kiaro::Support::BitStream &in);
                    virtual void instantiate(void) = 0;
                    virtual void update(const Kiaro::Common::F32 &deltaTimeSeconds) = 0;

                // Protected Members
                protected:
                    const Kiaro::Common::U32 mTypeMask;
                    Kiaro::Common::U32 mNetID;
                    Kiaro::Common::Vector3DF mPosition;
                    irr::scene::ISceneNode *mSceneNode;
            };
        } // End Namespace Entities
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_ENTITIES_ENTITYBASE_HPP_
