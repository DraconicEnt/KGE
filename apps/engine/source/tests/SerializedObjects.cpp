/**
 *  @file SerializedObjects.cpp
 *  @brief Serialized Objects testing implementation.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <engine/config.hpp>

#if ENGINE_TESTS>0
    #ifndef _INCLUDE_KIARO_TESTS_SERIALIZEDOBJECTS_H_
        #define _INCLUDE_KIARO_TESTS_SERIALIZEDOBJECTS_H_

        #include <gtest/gtest.h>

        #include <engine/INetworkPersistable.hpp>

        // The Test class
        class TestSerializedObject : public Kiaro::Engine::INetworkPersistable
        {
            // Public Methods
            public:
                //! Default Constructor
                TestSerializedObject(void) : mNetworkedUInt(0)
                {
                    addNetworkedProperty("networkeduint", mNetworkedUInt);
                }

                void packData(Kiaro::Support::CBitStream &out) { }
                void unpackData(Kiaro::Support::CBitStream &in) { }

            // Public Members
            public:
                unsigned int mNetworkedUInt;
        };

        TEST(SerializedObjectTest, Initialization)
        {
            TestSerializedObject test = TestSerializedObject();
        }

        TEST(SerializedObjectTest, SettingProperties)
        {
            TestSerializedObject test = TestSerializedObject();

            test.setNetworkedPropertyValue("networkeduint", 15u);

            EXPECT_EQ(15u, test.mNetworkedUInt);
            EXPECT_EQ(1, test.mDirtyNetworkedProperties.size());
        }

        TEST(SerializedObjectTest, SettingInvalidProperties)
        {
            TestSerializedObject test = TestSerializedObject();

            // Check for invalid type
            bool raisedException = false;
            try { test.setNetworkedPropertyValue("networkeduint", 3.14159f); }
            catch (std::logic_error &exception) { raisedException = true; }

            EXPECT_EQ(true, raisedException);

            // Check for invalid key
            raisedException = false;

            try { test.setNetworkedPropertyValue("someunknownproperty", 3.14159f); }
            catch (std::logic_error &exception) { raisedException = true; }

            EXPECT_EQ(true, raisedException);
        }

        TEST(SerializedObjectTest, GettingProperties)
        {
            TestSerializedObject test = TestSerializedObject();
            test.setNetworkedPropertyValue("networkeduint", 15u);

            EXPECT_EQ(15u, test.getNetworkedPropertyValue<unsigned int>("networkeduint"));
        }

        TEST(SerializedObjectTest, GettingInvalidProperties)
        {
            TestSerializedObject test = TestSerializedObject();
            test.setNetworkedPropertyValue("networkeduint", 15u);

            // Invalid type
            bool raisedException = false;
            try { test.getNetworkedPropertyValue<Kiaro::Common::F32>("networkeduint"); }
            catch (std::logic_error &exception) { raisedException = true; }

            EXPECT_EQ(true, raisedException);

            // Invalid key
            raisedException = false;
            try { test.getNetworkedPropertyValue<Kiaro::Common::F32>("someunknownproperty"); }
            catch (std::logic_error &exception) { raisedException = true; }

            EXPECT_EQ(true, raisedException);
        }
    #endif // _INCLUDE_KIARO_TESTS_H_
#endif // _INCLUDE_KIARO_TESTS_SERIALIZEDOBJECTS_H_
