load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def mygui():
    # Load MyGUI
    maybe(
        new_git_repository,
        name = "mygui",
        remote = "https://github.com/MyGUI/mygui.git",
        commit = "42594b05f87f9cfa6ade9fe4c296ec49b2c9982e", # Tag MyGUI3.4.0
        shallow_since = "1581378518 +0300",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "library",
   lib_source = "@mygui//:.",

   deps = [
      # "@sdl2_image//:sdl2_image"
      "@freetype//:freetype",
      "@sdl2//:library",
      "@sdl2_image_prebuilt//:sdl2_image_prebuilt"
   ],

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

   cache_entries = {
       "FREETYPE_DIR": "$EXT_BUILD_DEPS/freetype",
       "MYGUI_INSTALL_DOCS": "FALSE",
        "SDL2_INCLUDE_DIR": "$EXT_BUILD_DEPS/sdl2/include",
        "SDL2_LIBRARY": "$EXT_BUILD_DEPS/sdl2/lib/sdl2.lib",

       "SDL2_IMAGE_LIBRARY": "$EXT_BUILD_DEPS/sdl2_image/lib/libSDL2_image.a",
       "SDL2_IMAGE_INCLUDE_DIR": "$EXT_BUILD_DEPS/sdl2_image/include/SDL2",
       "MYGUI_BUILD_DEMOS": "FALSE",
       "MYGUI_BUILD_UNITTESTS": "FALSE",
       "MYGUI_BUILD_TEST_APP": "FALSE",
       "MYGUI_RENDERSYSTEM": "7"
   },

 #  static_libraries = [
  #     "libCommon.a",
  #     "libMyGUI.OpenGL3Platform.a"
  # ],

   shared_libraries = [
       "libMyGUIEngine.so",
       "libEditorFramework.so"
   ]
)
        """
    )
