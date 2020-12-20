load("@rules_pkg//:pkg.bzl", "pkg_tar")

cc_binary(
    name = "main",
    srcs = [
        "source/main.cpp"
    ],
    deps = [
        "//components/engine:engine",

        "@allegro//:allegro",
        "@osg//:osg",
        "@physfs//:physfs",
        "@bullet//:bullet",
        "@enet//:enet"
    ],
    linkopts = [
        # "-lpthread"
    ],
    copts = [
        "-Ibazel-out/k8-fastbuild/bin/external/bullet/bullet/include/bullet"
    ],
    visibility = ["//visibility:public"]
)
