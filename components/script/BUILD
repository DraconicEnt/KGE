"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "script",
    srcs = glob(
        include=[
            "include/**/*.hpp",

            # For now we force lua bindings
            "bindings/lua/**/*.cpp",
            "bindings/lua/**/*.hpp"
        ],
    ),

    includes = [
        "include",
        "bindings/lua/include",
    ],

    deps = [
        "//components/support:support",

        "@physfs//:physfs",
        "@lua//:lua",
        "@easylua//:easylua"
    ],

    visibility = ["//visibility:public"]
)
