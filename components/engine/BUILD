"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_third_party//applications:graphviz.bzl", "run_graphviz")

cc_library(
    name = "engine",
    srcs = select({
       "@bazel_tools//src/conditions:windows": glob(
           include = [
               "include/**/*.h",
               "include/**/*.hpp",

               "source/**/*.cpp"
           ]
       ),

       # Linux
       "//conditions:default": glob(
           include = [
               "include/**/*.h",
               "include/**/*.hpp",

               "source/**/*.cpp"
           ]
       )
    }),

    includes = [
        "include"
    ],

    linkopts = [
        "-lGL",
        "-lX11",
        "-lXxf86vm"
    ],

    copts = [
        "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
    ],
    defines = select({
        "//.bazel:mem-arena": ["ENGINE_ENTITY_ARENA_ALLOCATIONS=1"],
        "//.bazel:mem-malloc": [],
        "//conditions:default": []
    }) + [
        "_ENGINE_USE_GLOBAL_EXCEPTION_CATCH_=1"
    ],
    deps = [
        "@physfs//:physfs",
        "@allegro//:allegro",
        "@irrlicht//:irrlicht",

        "//components/phys:phys",
        "//components/net:net",
        "//components/sound:sound",
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
    expression = "deps(//components/engine:engine)",
    scope = ["//components/engine:engine"],
)

run_graphviz(
    name = "dependency_diagram_png",
    input = ":dependency_diagram",
    output = "dependency_diagram.png"
)
