/**
 *  @file ces.cpp
 *  @brief Source file containing coding for the ces tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <gtest/gtest.h>

#include <support/UnorderedMap.hpp>

#include <ces/ces.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace CES
        {
            TEST(CES, Iterator)
            {
                CES::CComponentRoot* root = new CES::CComponentRoot();

                // Need at least one renderable in the root -- note this won't actually instantiate anything as we're just defining the model.
                CES::CRenderModel* rendered = new CES::CRenderModel("path/to/some/model/data");

                // Establish that we want basic rigid body sim on the renderable
                CES::CRigidBody* simulation = new CES::CRigidBody("path/to/mesh/data/or/mesh/data");
                rendered->attachComponent(simulation);

                // We also want to express that our rigid body has some buoyancy
                CES::CBuoyancy* buoyancy = new CES::CBuoyancy(5.0f);
                simulation->attachComponent(buoyancy);

                // ... and is magnetic
                CES::CMagnetic* magnetic = new CES::CMagnetic(15.0f);
                simulation->attachComponent(magnetic);

                // Components are connected directly to each other, so it doesn't matter when root is finally given the renderable
                root->attachComponent(rendered);

                auto it = root->begin();
                EXPECT_EQ(rendered, *it);
                it = ++it;

                EXPECT_EQ(simulation, *it);
                it = ++it;

                EXPECT_EQ(buoyancy, *it);
                it = ++it;

                EXPECT_EQ(magnetic, *it);
                it = ++it;

                EXPECT_EQ(it, root->end());

                delete root;
            }

            TEST(CES, Children)
            {
                CES::CComponentRoot* root = new CES::CComponentRoot();

                // Need at least one renderable in the root -- note this won't actually instantiate anything as we're just defining the model.
                CES::CRenderModel* rendered = new CES::CRenderModel("path/to/some/model/data");

                // Establish that we want basic rigid body sim on the renderable
                CES::CRigidBody* simulation = new CES::CRigidBody("path/to/mesh/data/or/mesh/data");
                rendered->attachComponent(simulation);

                // We also want to express that our rigid body has some buoyancy
                CES::CBuoyancy* buoyancy = new CES::CBuoyancy(5.0f);
                simulation->attachComponent(buoyancy);

                // ... and is magnetic
                CES::CMagnetic* magnetic = new CES::CMagnetic(15.0f);
                simulation->attachComponent(magnetic);

                // Components are connected directly to each other, so it doesn't matter when root is finally given the renderable
                root->attachComponent(rendered);

                Support::Set<CES::IComponent*> components;
                root->getChildren(true, components);

                auto it = components.begin();

                EXPECT_EQ(rendered, *it);
                it = ++it;

                EXPECT_EQ(simulation, *it);
                it = ++it;

                EXPECT_EQ(buoyancy, *it);
                it = ++it;

                EXPECT_EQ(magnetic, *it);
                it = ++it;
                EXPECT_EQ(it, components.end());

                delete root;
            }

            TEST(CES, ChildrenIteratorEquality)
            {
                CES::CComponentRoot* root = new CES::CComponentRoot();

                // Need at least one renderable in the root -- note this won't actually instantiate anything as we're just defining the model.
                CES::CRenderModel* rendered = new CES::CRenderModel("path/to/some/model/data");

                // Establish that we want basic rigid body sim on the renderable
                CES::CRigidBody* simulation = new CES::CRigidBody("path/to/mesh/data/or/mesh/data");
                rendered->attachComponent(simulation);

                // We also want to express that our rigid body has some buoyancy
                CES::CBuoyancy* buoyancy = new CES::CBuoyancy(5.0f);
                simulation->attachComponent(buoyancy);

                // ... and is magnetic
                CES::CMagnetic* magnetic = new CES::CMagnetic(15.0f);
                simulation->attachComponent(magnetic);

                // Components are connected directly to each other, so it doesn't matter when root is finally given the renderable
                root->attachComponent(rendered);

                Support::Set<CES::IComponent*> components;
                root->getChildren(true, components);

                auto childrenIt = components.begin();
                auto rootIt = root->begin();

                EXPECT_EQ(*childrenIt, *rootIt);
                rootIt = ++rootIt;
                childrenIt = ++childrenIt;

                EXPECT_EQ(*childrenIt, *rootIt);
                rootIt = ++rootIt;
                childrenIt = ++childrenIt;

                EXPECT_EQ(*childrenIt, *rootIt);
                rootIt = ++rootIt;
                childrenIt = ++childrenIt;

                EXPECT_EQ(*childrenIt, *rootIt);
                rootIt = ++rootIt;
                childrenIt = ++childrenIt;

                EXPECT_EQ(rootIt, root->end());
                EXPECT_EQ(childrenIt, components.end());

                delete root;
            }

            TEST(CES, StrategyEquality)
            {
                /* Todo: */
            }
        } // End Namespace CES
    } // End NameSpace Engine
} // End namespace Kiaro

