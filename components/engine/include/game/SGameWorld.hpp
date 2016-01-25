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

#include <support/CBitStream.hpp>
#include <support/UnorderedSet.hpp>
#include <support/String.hpp>

#include <support/common.hpp>

namespace Kiaro
{
    namespace Game
    {
        class IGameMode;
        
        namespace Entities
        {
            class IEntity;
            class CSky;
        } // End NameSpace Entities

        class SGameWorld : public Support::ISerializable
        {
            friend class Entities::IEntity;
            
            // Private Members
            private:
                Support::UnorderedSet<Entities::IEntity*> mEntities;

                Entities::CSky* mSky;
                
                IGameMode* mGameMode;
                
            // Public Members
            public:
                typedef Support::UnorderedSet<Entities::IEntity*>::iterator iterator;
                typedef Support::UnorderedSet<Entities::IEntity*>::const_iterator const_iterator;

            // Public Methods
            public:
                static SGameWorld* getPointer(void);
                static void destroy(void);
                
                void addEntity(Entities::IEntity* entity);
                void removeEntity(Entities::IEntity* entity);
                void removeEntity(const Common::U32& id);
                
                Entities::IEntity* getEntity(const Common::U32& id) const;
                Entities::IEntity* getEntity(const Support::String& name) const;

                void update(const Common::F32& deltaTimeSeconds);
                void clear(void);
                const Entities::CSky* getSky(void);
                
                void packEverything(Support::CBitStream& out) const;
                void unpack(Support::CBitStream& in);
                
                void setGameMode(IGameMode* game);
                IGameMode* getGameMode(void);
                
                iterator begin(void);
                const_iterator end(void);

            // Private Methods
            private:
                //! Standard constructor
                SGameWorld(void);
                //! Standard destructor
                ~SGameWorld(void);
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_SGAMEWORLD_HPP_
