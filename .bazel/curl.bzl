
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "curl",
   lib_source = "@curl//:curl-7.73.0",
   shared_libraries = [
       "libcurl.so"
   ],
   make_commands = [
       "make -j$(nproc)",
       "make install"
   ],
   visibility = ["//visibility:public"]
)
