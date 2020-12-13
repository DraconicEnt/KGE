
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "allegro",
   lib_source = "@allegro//:.",

   cache_entries = {
   "PHYSFS_INCLUDE_DIR": "$EXT_BUILD_DEPS/physfs/include",
   "PHYSFS_LIBRARY": "$EXT_BUILD_DEPS/physfs/lib/libphysfs.a",
   },

    make_commands = [
       "make -j$(nproc)",
       "make install"
    ],

    deps = [
        "@physfs//:physfs"
    ],

   shared_libraries = [
       "liballegro_physfs.so",
       "liballegro_physfs.so.5.2",
       "liballegro_acodec.so",
       "liballegro_acodec.so.5.2",
      # "liballegro_dialog.so",
       "liballegro_color.so",
       "liballegro_color.so.5.2",
       "liballegro_ttf.so",
       "liballegro_ttf.so.5.2",
       "liballegro_font.so",
       "liballegro_font.so.5.2",
       "liballegro_image.so",
       "liballegro_image.so.5.2",
       "liballegro_main.so",
       "liballegro_main.so.5.2",
       "liballegro.so",
       "liballegro.so.5.2",
       "liballegro_audio.so",
       "liballegro_audio.so.5.2",
       "liballegro_memfile.so",
       "liballegro_memfile.so.5.2",
       "liballegro_primitives.so",
       "liballegro_primitives.so.5.2"
   ],

   visibility = ["//visibility:public"]
)
