genrule(
    name = "test",
    cmd = "ls bazel-out/x64_windows-fastbuild/bin/external/bullet/bullet/include/bullet",
    srcs = [
        "@bullet//:bullet"
    ],
    outs = [
        "ls"
    ]
)

cc_library(
    name = "phys",
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
    copts =  select({
        "@bazel_tools//src/conditions:windows": [

        ],
        "//conditions:default": [
            "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
        ],
    }),

    #include_prefix = "bullet",
    deps = [
        "@bullet//:bullet",

        "//components/support:support"
    ],
    visibility = ["//visibility:public"]
)
