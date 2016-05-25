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

                TestClass(void) : mFunctionCalled(false) {}

                void testFunction(const Support::Vector<Support::String>& params)
                {
                    mFunctionCalled = true;
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

            // FIXME: Calls against member bindings causes a segfault right now
          //  console.eval("testClass");

          //  EXPECT_TRUE(testInstance.mFunctionCalled);
        }
    } // End Namespace Support
} // End namespace Kiaro

