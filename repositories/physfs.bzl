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

static_libraries = select({
   "@bazel_tools//src/conditions:windows": [
       "physfs.lib",
       "physfs-static.lib"
   ],

   # Linux
   "//conditions:default": [
      "libphysfs.a",
   ]
}),

shared_libraries = select({
   "@bazel_tools//src/conditions:windows": [
        "physfs.dll"
   ],

   # Linux
   "//conditions:default":[
      "libphysfs.so",
      "libphysfs.so.1"
   ]
}),


generate_crosstool_file = select({
    "@bazel_tools//src/conditions:windows": True,
    "//conditions:default": False
}),

cmake_options = select({
   "@bazel_tools//src/conditions:windows": ["-GNinja"],
   "//conditions:default": None
}),

make_commands = select({
    "@bazel_tools//src/conditions:windows": [
        "ninja",
        # Ninja install is broken
        "mkdir -p physfs/lib",
        "cp physfs.dll physfs/lib",
        "cp physfs.lib physfs/lib",
        "cp physfs-static.lib physfs/lib",
   ],
   "//conditions:default": [
       "make -j$(nproc)",
       "make install"
   ]
}),
visibility = ["//visibility:public"]
)
        """
    )
