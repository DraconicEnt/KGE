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
            "source/SSignalHandler.cpp"
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
