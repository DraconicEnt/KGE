cc_library(
    name = "support",
    srcs = glob(
        include=[
            "source/**/*.cpp",
            "source/**/*.c",
            "include/**/*.h",
            "include/**/*.hpp",

            "platform/common/**/*.cpp"
        ]) + select({
            "@platforms//os:windows": glob(include=["platform/windows/**/*.cpp"]), 
            "//conditions:default": glob(include=["platform/unix/**/*.cpp"])
        }),
    includes = [
        "include"
    ],

    deps = [
        "//deps/easydelegate:easydelegate",
        "@allegro//:allegro"
    ],

    visibility = ["//visibility:public"]
)