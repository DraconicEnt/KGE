"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "support",
    srcs = glob(
        include=[
            "source/**/*.cpp",
            "source/**/*.c",
            "include/**/*.h",
            "include/**/*.hpp",

            "platform/common/**/*.cpp"
        ],
        exclude =[

        ]) + select({
            "@platforms//os:windows": glob(include=["platform/windows/**/*.cpp"]),
            "//conditions:default": glob(include=["platform/unix/**/*.cpp"])
        }),
    includes = [
        "include"
    ],

    defines = select({
        "//conditions:default": [],
        "//.bazel:intrinsics": [],
        "//.bazel:no-intrinsics": ["NO_INTRINSICS=1"]
    }),

    linkopts = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["-lpthread", "-ldl"]
    }),

    hdrs = glob(
        include = [
            "external/physfs/physfs-3.0.2/src/*.h"
        ]
    ),

    deps = [
        "@physfs//:physfs",
        "@easydelegate//:easydelegate",
        "@allegro//:allegro",
        "@osg//:osg"
    ],

    visibility = ["//visibility:public"]
)
