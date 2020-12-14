load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def sdl2():
    # Load SDL2
    maybe(
        http_archive,
        name = "sdl2",
        urls = [
            "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
        ],
        sha256 = "349268f695c02efbc9b9148a70b85e58cefbbf704abd3e91be654db7f1e2c863",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
name = "library",
lib_source = "@sdl2//:SDL2-2.0.12",

static_libraries = [
   "libSDL2.a",
   "libSDL2main.a"
#   "SDL2.lib",
 #  "SDL2main.lib"
],
#shared_libraries = [
 #  "libSDL2-2.0.so"
#],

cache_entries = {
   "LIBC": "YES",
   "SDL_TEST": "NO",
   "SDL_SHARED": "NO",
   "SDL_STATIC": "YES",
},

# Windows only
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
       "ninja install"
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
