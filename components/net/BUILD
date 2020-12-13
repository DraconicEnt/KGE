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
