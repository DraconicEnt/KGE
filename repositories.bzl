load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def repositories():
    # Foreign CC
    maybe(
        http_archive,
        name = "rules_foreign_cc",
        strip_prefix = "rules_foreign_cc-master",
        sha256 = "3e6b0691fc57db8217d535393dcc2cf7c1d39fc87e9adb6e7d7bab1483915110",
        url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
    )

    # Load EasyDelegate
    maybe(
        git_repository,
        name = "easydelegate",
        shallow_since = "1607857580 -0800",
        remote = "https://github.com/Ragora/EasyDelegate.git",
        commit = "c94ca0096212ffa43aad9ca0aa5183fb7ebd742a"
    )

    # Load rules_python
    #http_archive(
    #    name = "rules_python",
    #    url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
    #    sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
    #)

    #load("@rules_python//python:pip.bzl", "pip_repositories")
    #pip_repositories()

    maybe(
        http_archive,
        name = "tiff",
        url = "https://download.osgeo.org/libtiff/tiff-4.1.0.tar.gz",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "tiff",
   lib_source = "@tiff//:tiff-4.1.0",

   static_libraries = [
       "tiff.lib"
   ],

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],

   visibility = ["//visibility:public"]
)
        """
    )


    maybe(
        http_archive,
        name = "zlib",
        url = "https://zlib.net/zlib-1.2.11.tar.gz",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "zlib",
   lib_source = "@zlib//:zlib-1.2.11",

   static_libraries = [
       "zlib.lib"
   ],

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],

   visibility = ["//visibility:public"]
)
        """
    )

    maybe(
        http_archive,
        name = "png",
        url = "https://downloads.sourceforge.net/project/libpng/libpng16/1.6.37/libpng-1.6.37.tar.xz?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Flibpng%2Ffiles%2Flibpng16%2F1.6.37%2Flibpng-1.6.37.tar.xz%2Fdownload&ts=1607941742",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
name = "png",
lib_source = "@png//:libpng-1.6.37",

cache_entries = {
    "ZLIB_LIBRARY": "$EXT_BUILD_DEPS/zlib/lib/zlibstatic.lib",
    "ZLIB_INCLUDE_DIR": "$EXT_BUILD_DEPS/zlib/include"
},

static_libraries = [
   "png.lib"
],

deps = [
    "@zlib//:zlib"
],

# Windows only
generate_crosstool_file = True,
cmake_options = ["-GNinja"],
make_commands = [
   "ninja",
   "ninja install",
],

visibility = ["//visibility:public"]
)
        """
    )

    maybe(
        http_archive,
        name = "freetype",
        url = "https://download.savannah.gnu.org/releases/freetype/freetype-2.9.1.tar.gz",
        sha256 = "ec391504e55498adceb30baceebd147a6e963f636eb617424bcfc47a169898ce",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "freetype",
   lib_source = "@freetype//:freetype-2.9.1",

   static_libraries = [
       "freetype.lib"
   ],

   deps = [
       "@zlib//:zlib"
   ],

   cache_entries = {
       "SKIP_INSTALL_HEADERS": "NO",
       "ZLIB_LIBRARY": "$EXT_BUILD_DEPS/zlib/lib/zlibstatic.lib",
       "ZLIB_INCLUDE_DIR": "$EXT_BUILD_DEPS/zlib/include"
   },

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],


   visibility = ["//visibility:public"]
)
        """
    )

    # Load rules_pkg
    maybe(
        http_archive,
        name = "rules_pkg",
        url = "https://github.com/bazelbuild/rules_pkg/releases/download/0.3.0/rules_pkg-0.3.0.tar.gz",
        sha256 = "6b5969a7acd7b60c02f816773b06fcf32fbe8ba0c7919ccdc2df4f8fb923804a",
    )

    # Load Google Test
    maybe(
        git_repository,
        name = "gtest",
        remote = "https://github.com/google/googletest.git",
        # 1.10.0
        commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
        shallow_since = "1570114335 -0400"
    )

    # Load Allegro
    maybe(
        new_git_repository,
        name = "allegro",
        remote = "https://github.com/liballeg/allegro5.git",
        # 5.2.6
        commit = "5e0db6b985eb2714fd276d8808b45f31ea6f52d9",
        shallow_since = "1581300180 -0800",
        build_file_content = """
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
        """
    )

    # Load CURL
    maybe(
        http_archive,
        name = "curl",
        urls = [
            "https://curl.se/download/curl-7.73.0.tar.gz"
        ],
        sha256 = "ba98332752257b47b9dea6d8c0ad25ec1745c20424f1dd3ff2c99ab59e97cf91",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "curl",
   lib_source = "@curl//:curl-7.73.0",
  # shared_libraries = [
       #"libcurl.so"
    #   "curl.lib"
   #],

   static_libraries = [
       "libcurl_imp.lib"
   ],

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],

   visibility = ["//visibility:public"]
)
        """
    )

    # Load SDL2_Image
    maybe(
        http_archive,
        name = "sdl2_image",
        urls = [
            "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.tar.gz"
        ],
        build_file_content = """
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
        """
    )

    # Load SDL2
    maybe(
        http_archive,
        name = "sdl2",
        urls = [
            "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
        ],
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "library",
   lib_source = "@sdl2//:SDL2-2.0.12",

   static_libraries = [
    #   "libSDL2.a"
       "SDL2.lib",
       "SDL2main.lib"
   ],
   #shared_libraries = [
    #   "libSDL2-2.0.so"
   #],

   cache_entries = {
       "LIBC": "YES",
       "SDL_TEST": "NO",
       "SDL_SHARED": "NO",
       "SDL_STATIC": "YES",
   },

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],

   visibility = ["//visibility:public"]
)
    """
    )

    # Load SDL2 Image prebuilt
    maybe(
        http_archive,
        name = "sdl2_image_prebuilt",
        url = "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-VC.zip",
        build_file_content = """
cc_import(
    name = "sdl2_image_prebuilt",
    hdrs = [
        "SDL2_image-2.0.5/include/SDL_image.h"
    ],
    static_library = "SDL2_image-2.0.5/lib/x64/SDL2_image.lib",
    visibility = ["//visibility:public"]
)
        """
    )

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

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],

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

    # Load Bullet
    maybe(
        new_git_repository,
        name = "bullet",
        remote = "https://github.com/bulletphysics/bullet3.git",
        # Tag 3.0.7
        commit = "272c7099d3ba8ac9e8c142e361e8d4cf5d91b8f2",
        shallow_since = "1606325146 -0800",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

filegroup(
    name = "bullet_files",
    srcs = glob(
        include=[
            "**/*"
        ]
    )
)
# export VS160COMNTOOLS="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools"
cmake_external(
   name = "bullet",
   lib_source = ":bullet_files",

  # make_commands = [
    #   "make -j$(nproc)",
     #  "make install",
   #],
   cache_entries = {
      "BUILD_BULLET3": "ON",
      "BUILD_PYBULLET": "OFF",
      "BUILD_EXTRAS": "OFF",
      "BUILD_EGL": "OFF",
      # Force built libs to be installed
      "INSTALL_LIBS": "ON",
      "BUILD_UNIT_TESTS": "OFF",
      "BUILD_OPENGL3_DEMOS": "OFF",
      "BUILD_BULLET2_DEMOS": "OFF"
   },

   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install",
   ],

   static_libraries = [
       #"libBulletDynamics.a",
       #"libBulletCollision.a",
       #"libBullet3Collision.a",
       #"libLinearMath.a",
       #"libBullet3Common.a",

       "BulletDynamics.lib",
       "BulletCollision.lib",
       "Bullet3Collision.lib",
       "LinearMath.lib",
       "Bullet3Common.lib",
   ],
   visibility = ["//visibility:public"]
)
        """
    )

    # Load OpenSceneGraph
    maybe(
        new_git_repository,
        name = "osg",
        remote = "https://github.com/openscenegraph/OpenSceneGraph.git",
        # Tag 3.6.5
        commit = "a827840baf0786d72e11ac16d5338a4ee25779db",
        shallow_since = "1580468587 +0000",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
    name = "osg",
    lib_source = "@osg//:.",
    shared_libraries = [
    "osgUI.dll",
    "osgParticle.dll",
    "osgGA.dll",
    "osgManipulator.dll",
    "osgShadow.dll",
    "osgWidget.dll",
    "osgPresentation.dll",
    "osgText.dll",
    "osgFX.dll",
    "osgTerrain.dll",
    "osgViewer.dll",
    "OpenThreads.dll",
    "osgVolume.dll",
    "osgUtil.dll",
    "osgSim.dll",
    "osgAnimation.dll",
    "osgDB.dll",
    "osg.dll",

        #"libosgUI.so",
        #"libosgParticle.so",
        #"libosgGA.so",
        #"libosgManipulator.so",
        #"libosgShadow.so",
        #"libosgWidget.so",
        #"libosgPresentation.so",
        #"libosgText.so",
        #"libosgFX.so",
        #"libosgTerrain.so",
        #"libosgViewer.so",
        #"libOpenThreads.so",
        #"libosgVolume.so",
        #"libosgUtil.so",
        #"libosgSim.so",
        #"libosgAnimation.so",
        #"libosgDB.so",
        #"libosg.so",

        #"libosgUI.so.161",
        #"libosgParticle.so.161",
        #"libosgGA.so.161",
        #"libosgManipulator.so.161",
        #"libosgShadow.so.161",
        #"libosgWidget.so.161",
        #"libosgPresentation.so.161",
        #"libosgText.so.161",
        #"libosgFX.so.161",
        #"libosgTerrain.so.161",
        #"libosgViewer.so.161",
        #"libOpenThreads.so.21",
        #"libosgVolume.so.161",
        #"libosgUtil.so.161",
        #"libosgSim.so.161",
        #"libosgAnimation.so.161",
        #"libosgDB.so.161",
        #"libosg.so.161"
   ],
   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       "ninja install"
   ],

   #make_commands = [
    #   "make -j$(nproc)",
     #  "make install"
   #],
   visibility = ["//visibility:public"]
)
    """
    )

    # Load ENet
    maybe(
        http_archive,
        name = "enet",
        urls = [
            "http://enet.bespin.org/download/enet-1.3.17.tar.gz"
        ],
        sha256 = "a38f0f194555d558533b8b15c0c478e946310022d0ec7b34334e19e4574dcedc",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")


cmake_external(
   name = "enet",
   lib_source = "@enet//:enet-1.3.17",
  # make_commands = [
    #   "make -j$(nproc)",
     #  "make install",
   #],


   # Windows only
   generate_crosstool_file = True,
   cmake_options = ["-GNinja"],
   make_commands = [
       "ninja",
       # Ninja install is not valid so we perform this operation ourself
       "mkdir -p enet/lib",
       "cp enet.lib enet/lib"
   ],

   static_libraries = [
       "enet.lib"
   ],
   visibility = ["//visibility:public"]
)


        """
    )

    # PhysicsFS
    maybe(
        http_archive,
        name = "physfs",
        urls = [
            "https://www.icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2"
        ],
        sha256 = "304df76206d633df5360e738b138c94e82ccf086e50ba84f456d3f8432f9f863",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "physfs",
   lib_source = "@physfs//:physfs-3.0.2",

   static_libraries = [
       "libphysfs.a",
   ],
   shared_libraries = [
       "libphysfs.so"
   ],
   make_commands = [
       "make -j$(nproc)",
       "make install",
   ],
   visibility = ["//visibility:public"]
)
        """
    )
