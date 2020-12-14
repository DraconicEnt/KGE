load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def freetype():
    maybe(
        http_archive,
        name = "freetype",
        url = "https://download.savannah.gnu.org/releases/freetype/freetype-2.9.1.tar.gz",
        sha256 = "ec391504e55498adceb30baceebd147a6e963f636eb617424bcfc47a169898ce",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
name = "freetype",
lib_source = "@freetype//:freetype-2.9.1",

static_libraries = [
   "freetype.lib"
],

deps = [
   "@zlib//:zlib"
],

cache_entries = {
   "SKIP_INSTALL_HEADERS": "NO",
   "ZLIB_LIBRARY": "$EXT_BUILD_DEPS/zlib/lib/zlibstatic.lib",
   "ZLIB_INCLUDE_DIR": "$EXT_BUILD_DEPS/zlib/include"
},

# Windows only
generate_crosstool_file = True,
cmake_options = ["-GNinja"],
make_commands = [
   "ninja",
   "ninja install",
],


visibility = ["//visibility:public"]
)
        """
    )
