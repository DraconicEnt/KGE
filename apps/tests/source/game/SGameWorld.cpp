/**
 *  @file SGameWorld.cpp
 *  @brief Source file containing coding for the SGameWorld tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/UnorderedMap.hpp>

#include <game/entities/IEntity.hpp>
#include <game/SGameWorld.hpp>

namespace Kiaro
{
    namespace Game
    {
        extern SGameWorld* sInstance;
        
        class TestEntity : public Entities::IEntity
        {
            public:
                TestEntity(void) : IEntity(Entities::ENTITY_NULL, Entities::NO_THINKING)
                {
                    instantiate();
                }

                TestEntity(Support::CBitStream& in) : IEntity(Entities::ENTITY_NULL, Entities::NO_THINKING)
                {
                    this->unpack(in);
                }

                ~TestEntity(void)
                {

                }

                void packEverything(Support::CBitStream& out) const
                {

                }

                void unpack(Support::CBitStream& in)
                {

                }

                void instantiate(void)
                {

                }

                void update(const Common::F32& deltaTimeSeconds)
                {

                }
        };
        
        TEST(SGameWorld, SingletonConstructor)
        {
            SGameWorld* world = SGameWorld::getPointer();
            
            EXPECT_TRUE(sInstance);
            
            SGameWorld::destroy();
            
            EXPECT_FALSE(sInstance);
        }
        
        TEST(SGameWorld, NameLookup)
        {
            SGameWorld* world = SGameWorld::getPointer();
            
            EXPECT_NO_THROW(EXPECT_FALSE(world->getEntity("someEntity")));
            
            TestEntity* entity = new TestEntity();            
            entity->setName("someEntity");
            
            EXPECT_NO_THROW(EXPECT_TRUE(world->getEntity("someEntity")));
            
            SGameWorld::destroy();
        }
    } // End Namespace Support
} // End namespace Kiaro

 
