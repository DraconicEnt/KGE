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
    include_prefix = "bullet",
    deps = [
        "@bullet//:bullet",

        "//components/support:support"
    ],
    visibility = ["//visibility:public"]
)
