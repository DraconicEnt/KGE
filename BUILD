load("@rules_pkg//:pkg.bzl", "pkg_tar")

load("//:dependencies.bzl", "generate_dependencies")

# TODO: Alternative for Windows deps
generate_dependencies(name="osg_dependencies", target="@osg//:osg", dependencies=[
    "libOpenThreads.so.21",
    "libosg.so.161",
    "libosgAnimation.so.161",
    "libosgDB.so.161",
    "libosgFX.so.161",
    "libosgGA.so.161",
    "libosgManipulator.so.161",
    "libosgParticle.so.161",
    "libosgPresentation.so.161",
    "libosgShadow.so.161",
    "libosgSim.so.161",
    "libosgTerrain.so.161",
    "libosgText.so.161",
    "libosgUI.so.161",
    "libosgWidget.so.161",
    "libosgUtil.so.161",
    "libosgViewer.so.161",
    "libosgVolume.so.161"
])

generate_dependencies(name="allegro_dependencies", target="@allegro//:allegro", dependencies=[
    "liballegro.so.5.2",
    "liballegro_ttf.so.5.2",
    "liballegro_main.so.5.2",
    "liballegro_font.so.5.2",
    "liballegro_image.so.5.2",
    "liballegro_color.so.5.2",
    "liballegro_audio.so.5.2",
    "liballegro_physfs.so.5.2",
    "liballegro_acodec.so.5.2",
    "liballegro_memfile.so.5.2",
    "liballegro_primitives.so.5.2"
])

pkg_tar(
    name = "dependencies_packaged",
    srcs = [
        ":osg_dependencies",
        ":allegro_dependencies"
    ],
    package_dir = "lib",
)

pkg_tar(
    name = "kge_binary",
    srcs = [
        "run.sh",
        "//apps/main:main"
    ],
    deps = [
        ":dependencies_packaged"
    ],
    mode = "0755",
)
