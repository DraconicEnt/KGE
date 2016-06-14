/**
 *  @file main.cpp
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
    RUN_ALL_TESTS();

    // Cleanup
    PHYSFS_deinit();
    return 0;
}
