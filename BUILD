load("@rules_pkg//:pkg.bzl", "pkg_tar")

filegroup(
    name = "allegro_shared_libraries",
    srcs = [
        "@allegro//:allegro",
        #"@osg//:osg",
        #"@physfs//:physfs",
        #"@bullet//:bullet",
        #"@enet//:enet"
    ],
    output_group = "gen_dir"
)

pkg_tar(
    name = "allegro_shared_libraries_packaged",
    srcs = [
        ":allegro_shared_libraries"
    ],
    remap_paths = {
        "external/allegro/copy_allegro/allegro": "allegro"
    },
    strip_prefix = "SDF"
)

print("@allegro//:allegro")

pkg_tar(
    name = "kge_binary",
    srcs = [
        "//apps/main:main",
        ":kge_dependencies"
    ],
    mode = "0755",
)
