load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def osg():
    # Load OpenSceneGraph
    maybe(
        new_git_repository,
        name = "osg",
        remote = "https://github.com/openscenegraph/OpenSceneGraph.git",
        # Tag 3.6.5
        commit = "a827840baf0786d72e11ac16d5338a4ee25779db",
        shallow_since = "1580468587 +0000",
        build_file_content = """
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
name = "osg",
lib_source = "@osg//:.",
shared_libraries = [
    "libosgUI.so",
    "libosgParticle.so",
    "libosgGA.so",
    "libosgManipulator.so",
    "libosgShadow.so",
    "libosgWidget.so",
    "libosgPresentation.so",
    "libosgText.so",
    "libosgFX.so",
    "libosgTerrain.so",
    "libosgViewer.so",
    "libOpenThreads.so",
    "libosgVolume.so",
    "libosgUtil.so",
    "libosgSim.so",
    "libosgAnimation.so",
    "libosgDB.so",
    "libosg.so",

    "libosgUI.so.161",
    "libosgParticle.so.161",
    "libosgGA.so.161",
    "libosgManipulator.so.161",
    "libosgShadow.so.161",
    "libosgWidget.so.161",
    "libosgPresentation.so.161",
    "libosgText.so.161",
    "libosgFX.so.161",
    "libosgTerrain.so.161",
    "libosgViewer.so.161",
    "libOpenThreads.so.21",
    "libosgVolume.so.161",
    "libosgUtil.so.161",
    "libosgSim.so.161",
    "libosgAnimation.so.161",
    "libosgDB.so.161",
    "libosg.so.161"
],

generate_crosstool_file = select({
    "@bazel_tools//src/conditions:windows": True,
    "//conditions:default": False
}),

cmake_options = select({
   "@bazel_tools//src/conditions:windows": ["-GNinja"],
   "//conditions:default": None
}),

make_commands = select({
   "@bazel_tools//src/conditions:windows": [
       "ninja",
       "ninja install"
   ],
   "//conditions:default": [
       "make -j$(nproc)",
       "make install"
   ]
}),

visibility = ["//visibility:public"]
)
    """
    )
