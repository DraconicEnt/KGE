load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def bullet():
    # Load Bullet
    maybe(
        new_git_repository,
        name = "bullet",
        remote = "https://github.com/bulletphysics/bullet3.git",
        # Tag 3.0.7
        commit = "272c7099d3ba8ac9e8c142e361e8d4cf5d91b8f2",
        shallow_since = "1606325146 -0800",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

filegroup(
name = "bullet_files",
srcs = glob(
    include=[
        "**/*"
    ]
)
)
# export VS160COMNTOOLS="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools"
cmake_external(
name = "bullet",
lib_source = ":bullet_files",

# make_commands = [
#   "make -j$(nproc)",
 #  "make install",
#],
cache_entries = {
  "BUILD_BULLET3": "ON",
  "BUILD_PYBULLET": "OFF",
  "BUILD_EXTRAS": "OFF",
  "BUILD_EGL": "OFF",
  # Force built libs to be installed
  "INSTALL_LIBS": "ON",
  "BUILD_UNIT_TESTS": "OFF",
  "BUILD_OPENGL3_DEMOS": "OFF",
  "BUILD_BULLET2_DEMOS": "OFF"
},

# Windows only
generate_crosstool_file = True,
cmake_options = ["-GNinja"],
make_commands = [
   "ninja",
   "ninja install",
],

static_libraries = [
   "libBulletDynamics.a",
   "libBulletCollision.a",
   "libBullet3Collision.a",
   "libLinearMath.a",
   "libBullet3Common.a",

   #"BulletDynamics.lib",
   #"BulletCollision.lib",
   #"Bullet3Collision.lib",
   #"LinearMath.lib",
   #"Bullet3Common.lib",
],
visibility = ["//visibility:public"]
)
        """
    )