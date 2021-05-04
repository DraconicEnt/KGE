"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_third_party//applications:graphviz.bzl", "run_graphviz")

cc_library(
    name = "net",
    srcs = glob(
        include = [
            "include/**/*.h",
            "include/**/*.hpp",
            "source/**/*.cpp"
        ]
    ),
    includes = [
        "include"
    ],

    deps = [
        "@curl//:curl",
        "@enet//:enet",
        "//components/support:support"
    ],
    visibility = ["//visibility:public"]
)

genquery(
    name = "dependency_diagram",
    opts = [
        "--output",
        "graph"
    ],
    expression = "deps(//components/net:net)",
    scope = ["//components/net:net"],
)

run_graphviz(
    name = "dependency_diagram_png",
    input = ":dependency_diagram",
    output = "dependency_diagram.png"
)
