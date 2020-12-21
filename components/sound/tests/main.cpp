/**
 *  @file main.cpp
 *  @brief Main source file for all tests.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <physfs.h>
#include <gtest/gtest.h>

#include <support/common.hpp>

using namespace Kiaro;

/**
 *  @brief Standard entry point.
 *  @param arg A Common::S32 representing the total number of arguments passed to the program.
 *  @param argv An array of Kiaro::Common::C8 representing the parameters passed in to the program.
 *  @return A Common::S32 representing the exit code.
 */
Common::S32 main(Common::S32 argc, Common::C8* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    // Before we run the tests, we need to initialize PhysicsFS
    PHYSFS_init(argv[0]);
    PHYSFS_setSaneConfig("Draconic Entity", "KGE", "ZIP", 0, 0);
    Common::S32 result = RUN_ALL_TESTS();

    // Cleanup
    PHYSFS_deinit();
    return result;
}
