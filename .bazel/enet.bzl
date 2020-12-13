
load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
   name = "enet",
   lib_source = "@enet//:enet-1.3.17",

   static_libraries = [
       "libenet.a",
   ],

   make_commands = [
       "make -j$(nproc)",
       "make install"
   ],

   visibility = ["//visibility:public"]
)
