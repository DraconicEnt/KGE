load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def physfs():
    # PhysicsFS
    maybe(
        http_archive,
        name = "physfs",
        urls = [
            "https://www.icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2"
        ],
        sha256 = "304df76206d633df5360e738b138c94e82ccf086e50ba84f456d3f8432f9f863",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
name = "physfs",
lib_source = "@physfs//:physfs-3.0.2",

static_libraries = [
   "libphysfs.a",
],
shared_libraries = [
   "libphysfs.so"
],
make_commands = [
   "make -j$(nproc)",
   "make install",
],
visibility = ["//visibility:public"]
)
        """
    )
