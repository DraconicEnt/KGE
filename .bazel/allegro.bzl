
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "allegro",
   lib_source = "@allegro//:.",

   shared_libraries = [
       "liballegro_physfs.so",
       "liballegro_acodec.so",
      # "liballegro_dialog.so",
       "liballegro_color.so",
       "liballegro_ttf.so",
       "liballegro_font.so",
       "liballegro_image.so",
       "liballegro_main.so",
       "liballegro.so",
       "liballegro_audio.so",
       "liballegro_memfile.so",
       "liballegro_primitives.so"
   ],

   visibility = ["//visibility:public"]
)
