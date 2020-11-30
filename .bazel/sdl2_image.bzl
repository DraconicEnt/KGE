
load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
   name = "sdl2_image",
   lib_source = "@sdl2_image//:SDL2_image-2.0.5",

   static_libraries = [
       "libSDL2_image.a",
   ],

   shared_libraries = [
       "libSDL2_image.so"
   ],

   make_commands = [
       "make -j$(nproc)",
       "make install"
   ],

   visibility = ["//visibility:public"]
)
