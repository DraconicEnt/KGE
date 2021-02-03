"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_pkg//:pkg.bzl", "pkg_tar")

load("//:dependencies.bzl", "generate_dependencies")

generate_dependencies(name="osg_dependencies_windows", target="@osg//:osg", dependencies=[
    "osg161-osgViewer.dll",
    "osg161-osg.dll",
    "osg161-osgText.dll",
    "osg161-osgUtil.dll",
    "osg161-osgGA.dll",
    "osg161-osgDB.dll",
    "ot21-OpenThreads.dll"
])

generate_dependencies(name="osg_dependencies_linux", target="@osg//:osg", dependencies=[
    "libOpenThreads.so.21",
    "libosg.so.161",
    "libosgAnimation.so.161",
    "libosgDB.so.161",
    "libosgFX.so.161",
    "libosgGA.so.161",
    "libosgManipulator.so.161",
    "libosgParticle.so.161",
    "libosgPresentation.so.161",
    "libosgShadow.so.161",
    "libosgSim.so.161",
    "libosgTerrain.so.161",
    "libosgText.so.161",
    "libosgUI.so.161",
    "libosgWidget.so.161",
    "libosgUtil.so.161",
    "libosgViewer.so.161",
    "libosgVolume.so.161"
])

generate_dependencies(name="allegro_dependencies_windows", target="@allegro//:allegro", dependencies=[
    "allegro-5.2.dll",
    "allegro_physfs-5.2.dll"
])

generate_dependencies(name="allegro_dependencies_linux", target="@allegro//:allegro", dependencies=[
    "liballegro.so.5.2",
    "liballegro_ttf.so.5.2",
    "liballegro_main.so.5.2",
    "liballegro_font.so.5.2",
    "liballegro_image.so.5.2",
    "liballegro_color.so.5.2",
    "liballegro_audio.so.5.2",
    "liballegro_physfs.so.5.2",
    "liballegro_acodec.so.5.2",
    "liballegro_memfile.so.5.2",
    "liballegro_primitives.so.5.2"
])

pkg_tar(
    name = "dependencies_packaged",
    srcs = select({
        "@bazel_tools//src/conditions:windows": [
            ":osg_dependencies_windows",
            ":allegro_dependencies_windows"
        ],
        "//conditions:default": [
            ":osg_dependencies_linux",
            ":allegro_dependencies_linux"
        ]
    }),

    package_dir = select({
       "@bazel_tools//src/conditions:windows": ".",

       # Linux
       "//conditions:default": "lib"
    })
)

pkg_tar(
    name = "kge_binary",
    srcs = [
        "//apps/main:main"
    ] + select({
        "@bazel_tools//src/conditions:windows": [

        ],
        "//conditions:default": [
            "run.sh"
        ]
    }),
    deps = [
        ":dependencies_packaged"
    ],
    mode = "0755",
)

sh_binary(
    name = "doxygen",
    srcs = [
        "doxygen.sh"
    ],
    args = [
        "$(locations @doxygen//:doxygen)",
        "$(locations @graphviz//:graphviz)"
    ],
    data = [
        "@doxygen//:doxygen",
        "@graphviz//:graphviz",
        "doxyfile"
    ]
)

test_suite(
    name = "test_all",
    tests = [
        "//components/support/tests:tests",
        "//components/net/tests:tests",
        "//components/sound/tests:tests"
    ]
)
