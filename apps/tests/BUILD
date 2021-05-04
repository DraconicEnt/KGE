"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "tests",
    srcs = glob(
        include=["source/**/*.cpp"],
    ),
    deps = [
        "//components/phys:phys",
        "//components/support:support",
        "//components/net:net",
        "//components/sound:sound",
        "//components/engine:engine",

        # "@sdl2//:library",
        "@physfs//:physfs",
        "@bullet//:bullet",
        "@allegro//:allegro",
        "@enet//:enet",
        "@gtest//:gtest"
    ],
    linkopts = [
        "-ldl"
    ],
    copts = [
        "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
    ]
)
