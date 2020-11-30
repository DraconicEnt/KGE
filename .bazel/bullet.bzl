
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

"""
./src/Bullet3OpenCL/libBullet3OpenCL_clew.a
./src/Bullet3Geometry/libBullet3Geometry.a
./src/Bullet3Serialize/Bullet2FileLoader/libBullet2FileLoader.a
./src/Bullet3Dynamics/libBullet3Dynamics.a
./src/BulletSoftBody/libBulletSoftBody.a
./src/BulletInverseDynamics/libBulletInverseDynamics.a
./src/LinearMath/libLinearMath.a
./src/Bullet3Common/libBullet3Common.a
./src/BulletCollision/libBulletCollision.a
./src/BulletDynamics/libBulletDynamics.a
./src/Bullet3Collision/libBullet3Collision.a
"""

cmake_external(
   name = "bullet",
   lib_source = "@bullet//:.",

   make_commands = [
       "make -j$(nproc)",
       "make install",
   ],

   static_libraries = [
       "libLinearMath.a",
       "libBullet3Common.a",
       "libBulletCollision.a",
       "libBulletDynamics.a",
       "libBullet3Collision.a"
   ],
   visibility = ["//visibility:public"]
)
