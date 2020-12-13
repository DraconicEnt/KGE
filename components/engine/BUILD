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
    copts = [
        "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
    ],
    defines = select({
        "//.bazel:mem-arena": ["ENGINE_ENTITY_ARENA_ALLOCATIONS=1"],
        "//.bazel:mem-malloc": [],
        "//conditions:default": []
    }),
    deps = [
        "//components/phys:phys",
        "//components/net:net",
        "//components/sound:sound",
        "//components/support:support"
    ],
    visibility = ["//visibility:public"]
)
