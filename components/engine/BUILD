cc_library(
    name = "engine",
    srcs = glob(
        include = [
            "include/**/*.h",
            "include/**/*.hpp",
            "platform/unix/**/*.cpp",

            "source/**/*.cpp"
        ]
    ),
    includes = [
        "include"
    ],
    deps = [
        "//components/phys:phys",
        "//components/net:net",
        "//components/sound:sound",
        "//components/support:support"
    ]
)
