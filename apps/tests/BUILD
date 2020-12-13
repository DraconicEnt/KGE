cc_test(
    name = "tests",
    srcs = glob(
        include=["source/**/*.cpp"],

        # FIXME: SGameState tests cause a crash
        exclude=["source/**/game/*.cpp"]
    ),
    deps = [
        "//components/phys:phys",
        "//components/support:support",
        "//components/net:net",
        "//components/sound:sound",
        "//components/engine:engine",

        "@osg//:osg",
        "@physfs//:physfs",
        "@bullet//:bullet",
        "@allegro//:allegro",
        "@enet//:enet",
        "@gtest//:gtest"
    ],
    linkopts = [
        "-ldl"
    ],
    copts = [
        "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
    ]
)
