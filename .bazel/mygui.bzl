load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "library",
   lib_source = "@mygui//:.",

   deps = [
       "@sdl2_image//:sdl2_image"
   ],

   make_commands = [
       "make -j$(nproc)",
       "make install"
   ],

   cache_entries = {
       "SDL2_IMAGE_LIBRARY": "$EXT_BUILD_DEPS/sdl2_image/lib/libSDL2_image.a",
       "SDL2_IMAGE_INCLUDE_DIR": "$EXT_BUILD_DEPS/sdl2_image/include/SDL2",
       "MYGUI_BUILD_DEMOS": "FALSE",
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
