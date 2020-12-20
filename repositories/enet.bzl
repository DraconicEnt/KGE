load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def enet():
    # Load ENet
    maybe(
        http_archive,
        name = "enet",
        urls = [
            "http://enet.bespin.org/download/enet-1.3.17.tar.gz"
        ],
        sha256 = "a38f0f194555d558533b8b15c0c478e946310022d0ec7b34334e19e4574dcedc",
        build_file_content = """
cc_library(
    name = "enet",
    srcs = glob(
        include = [
            "enet-1.3.17/*.c",
            "enet-1.3.17/*.h",
            "enet-1.3.17/include/enet/*.h"
        ]
    ),
    # NOTE: Autogen?
    defines = [
        "HAS_SOCKLEN_T=1"
    ],
    linkopts = [
        "Winmm.lib",
        "Ws2_32.lib"
    ],
    alwayslink = True,
    hdrs = glob(
        include = [
            "enet-1.3.17/include/enet/*.h"
        ]
    ),
    includes = [
        "enet-1.3.17/include"
    ],
    visibility = ["//visibility:public"]
)
        """
    )
