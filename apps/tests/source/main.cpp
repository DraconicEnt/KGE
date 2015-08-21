/**
 *  @file main.cpp
 */

#include <gtest/gtest.h>

#include <support/common.hpp>

using namespace Kiaro;

/**
 *  @brief Standard entry point.
 *  @param arg A Common::S32 representing the total number of arguments passed to the program.
 *  @param argv An array of Kiaro::Common::C8 representing the parameters passed in to the program.
 *  @return A Common::S32 representing the exit code.
 */
Common::S32 main(Common::S32 argc, Common::C8 *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    return 0;
}
