
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "library",
   lib_source = "@sdl2//:SDL2-2.0.12",

   static_libraries = [
       "libSDL2.a"
   ],
   shared_libraries = [
       "libSDL2-2.0.so"
   ],
   make_commands = [
       "make -j$(nproc)",
       "make install"
   ],
   visibility = ["//visibility:public"]
)
