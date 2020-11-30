
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "library",
   lib_source = "@physfs//:physfs-3.0.2",

   static_libraries = [
       "libphysfs.a",
   ],
   shared_libraries = [ 
       "libphysfs.so"
   ]
)