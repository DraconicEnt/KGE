"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "sound",
    srcs = glob(
        include = [
            "include/**/*.h",
            "include/**/*.hpp",

            "source/**/*.cpp",

            # Set backend
            "backends/null/**/*.h",
            "backends/null/**/*.hpp",
            "backends/null/**/*.cpp",
        ]
    ),
    includes = [
        "include",

        # Set backend
        "backends/null/include"
    ],

    deps = [
        "@physfs//:physfs",
        "//components/support:support"
    ],
    visibility = ["//visibility:public"]
)
