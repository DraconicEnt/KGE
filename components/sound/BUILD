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
