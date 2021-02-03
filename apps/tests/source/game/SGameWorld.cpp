/**
 *  @file SGameWorld.cpp
 *  @brief Source file containing coding for the SGameWorld tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/UnorderedMap.hpp>

#include <game/entities/IEntity.hpp>
#include <game/SGameWorld.hpp>
#include <core/SObjectRegistry.hpp>

#include <game/entities/entities.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            extern SGameWorld* sGameWorld;

            class TestEntity : public Entities::IEntity
            {
                public:
                    TestEntity(void) : IEntity(Entities::ENTITY_NULL, Entities::FLAG_THINKING | Entities::FLAG_UPDATING)
                    {
                        this->registerEntity();
                    }

                    TestEntity(Support::CBitStream& in) : IEntity(Entities::ENTITY_NULL, Entities::FLAG_THINKING | Entities::FLAG_UPDATING)
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

                    void update(const Common::F32 deltaTimeSeconds)
                    {
                    }

                    size_t getRequiredMemory(void) const
                    {
                        return 5;
                    }
            };

            class TestObject : public Game::IEngineObject
            {
                public:
                    TestObject(void) : IEngineObject()
                    {
                    }
            };

            TEST(SGameWorld, Lookup)
            {
                SGameWorld* world = SGameWorld::getInstance();
                EXPECT_NO_THROW(EXPECT_FALSE(world->getEntity("entity")));

                TestEntity* entity = new TestEntity();
                entity->setName("entity");
                entity->registerEntity();

                EXPECT_EQ(entity, world->getEntity("entity"));
                EXPECT_EQ(entity, world->getEntity(entity->mID));

                SGameWorld::destroy();
                Core::SObjectRegistry::destroy();
            }

            TEST(SGameWorld, IDAssignment)
            {
                SGameWorld* world = SGameWorld::getInstance();

                TestEntity* entityOne = new TestEntity();
                entityOne->registerEntity();
                EXPECT_EQ(0, entityOne->mID);
                TestEntity* entityTwo = new TestEntity();
                entityTwo->registerEntity();

                EXPECT_EQ(1, entityTwo->mID);
                TestEntity* entityThree = new TestEntity();
                entityThree->registerEntity();

                EXPECT_EQ(2, entityThree->mID);
                world->removeEntity(entityTwo);

                delete entityTwo;
                entityTwo = new TestEntity();

                EXPECT_EQ(1, entityTwo->mID);
                world->removeEntity(entityTwo);

                delete entityTwo;

                SGameWorld::destroy();
                Core::SObjectRegistry::destroy();
            }

            TEST(SGameWorld, TypeTesting)
            {
                SGameWorld* world = SGameWorld::getInstance();
                TestEntity* entity = new TestEntity();

                entity->setName("entity");
                entity->registerEntity();
                TestObject* object = new TestObject();

                object->setName("object");
                EXPECT_FALSE(world->getEntity("object"));

                EXPECT_FALSE(world->getEntity(object->mID));
                EXPECT_EQ(entity, world->getEntity("entity"));

                EXPECT_EQ(entity, world->getEntity(entity->mID));

                SGameWorld::destroy();
                Core::SObjectRegistry::destroy();
            }
        } // End Namespace Support
    }
} // End namespace Kiaro
