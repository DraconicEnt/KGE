"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "source/main.cpp"
    ],
    deps = [
        "//components/engine:engine",

        "@allegro//:allegro",
        "@osg//:osg",
        "@physfs//:physfs",
        "@bullet//:bullet",
        "@enet//:enet"
    ],
    linkopts = [
        # "-lpthread"
    ],
    copts = [
        "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
    ],
    visibility = ["//visibility:public"]
)
