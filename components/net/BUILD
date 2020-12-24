load("//repositories:graphviz.bzl", "run_graphviz")

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

genquery(
    name = "dependency_diagram",
    opts = [
        "--output",
        "graph"
    ],
    expression = "deps(//components/net:net)",
    scope = ["//components/net:net"],
)

run_graphviz(
    name = "dependency_diagram_png",
    input = ":dependency_diagram",
    output = "dependency_diagram.png"
)
