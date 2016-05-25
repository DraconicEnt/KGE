/**
 *  @file CCommandLineParser.cpp
 */

#include <gtest/gtest.h>

#include <support/CManagementConsole.hpp>

namespace Kiaro
{
    namespace Support
    {
        static bool sFunctionCalled = false;
        static Support::Vector<Support::String> sGotParams;

        static void testStatic(const Support::Vector<Support::String>& params)
        {
            sFunctionCalled = true;
            sGotParams = params;
        }

        class TestClass
        {
            public:
                bool mFunctionCalled;
                Support::Vector<Support::String> mGotParams;

                TestClass(void) : mFunctionCalled(false) {}

                void testFunction(const Support::Vector<Support::String>& params)
                {
                    mFunctionCalled = true;
                    mGotParams = params;
                }
        };

        TEST(CManagementConsole, FunctionRegistration)
        {
            sFunctionCalled = false;

            CManagementConsole console;

            TestClass testInstance;
            EXPECT_NO_THROW(console.registerFunction("testStatic", testStatic));
            EXPECT_NO_THROW(console.registerFunction("testClass", &testInstance, &TestClass::testFunction));

            // Test static calls
            EXPECT_TRUE(console.eval("            testStatic 1      2    3                 "));

            EXPECT_TRUE(sFunctionCalled);
            EXPECT_EQ(3, sGotParams.size());
            EXPECT_EQ("1", sGotParams[0]);
            EXPECT_EQ("2", sGotParams[1]);
            EXPECT_EQ("3", sGotParams[2]);

            // Test member calls
            console.eval("           testClass a        b          c");

            EXPECT_TRUE(testInstance.mFunctionCalled);
            EXPECT_EQ(3, testInstance.mGotParams.size());
            EXPECT_EQ("a", testInstance.mGotParams[0]);
            EXPECT_EQ("b", testInstance.mGotParams[1]);
            EXPECT_EQ("c", testInstance.mGotParams[2]);
        }
    } // End Namespace Support
} // End namespace Kiaro

