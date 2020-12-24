load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

# Load all external repositories - these are split up for readability
load("//repositories:enet.bzl", "enet")
load("//repositories:osg.bzl", "osg")
load("//repositories:tiff.bzl", "tiff")
load("//repositories:zlib.bzl", "zlib")
load("//repositories:png.bzl", "png")
load("//repositories:freetype.bzl", "freetype")
load("//repositories:allegro.bzl", "allegro")
load("//repositories:curl.bzl", "curl")
load("//repositories:sdl2_image.bzl", "sdl2_image")
load("//repositories:sdl2_image_prebuilt.bzl", "sdl2_image_prebuilt")
load("//repositories:sdl2.bzl", "sdl2")
load("//repositories:mygui.bzl", "mygui")
load("//repositories:bullet.bzl", "bullet")
load("//repositories:physfs.bzl", "physfs")
load("//repositories:xml2.bzl", "xml2")
load("//repositories:iconv.bzl", "iconv")
load("//repositories:lzma.bzl", "lzma")
load("//repositories:doxygen.bzl", "doxygen")
load("//repositories:graphviz.bzl", "graphviz")
load("//repositories:harfbuzz.bzl", "harfbuzz")
load("//repositories:irrlicht.bzl", "irrlicht")
load("//repositories:jpeg.bzl", "jpeg")
load("//repositories:bzip2.bzl", "bzip2")

def repositories():
    # Foreign CC
    maybe(
        http_archive,
        name = "rules_foreign_cc",
        strip_prefix = "rules_foreign_cc-master",
        sha256 = "3e6b0691fc57db8217d535393dcc2cf7c1d39fc87e9adb6e7d7bab1483915110",
        url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
    )

    # Load EasyDelegate
    maybe(
        git_repository,
        name = "easydelegate",
        shallow_since = "1607857580 -0800",
        remote = "https://github.com/Ragora/EasyDelegate.git",
        commit = "68f60feb91025bc354ae67271f5e671800f782cd"
    )

    # Load rules_python
    #http_archive(
    #    name = "rules_python",
    #    url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
    #    sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
    #)

    #load("@rules_python//python:pip.bzl", "pip_repositories")
    #pip_repositories()


    zlib()
    png()
    tiff()
    jpeg()
    enet()
    iconv()
    bzip2()
    freetype()
    lzma()
    irrlicht()
    graphviz()
    harfbuzz()

    # Load rules_pkg
    maybe(
        http_archive,
        name = "rules_pkg",
        url = "https://github.com/bazelbuild/rules_pkg/releases/download/0.3.0/rules_pkg-0.3.0.tar.gz",
        sha256 = "6b5969a7acd7b60c02f816773b06fcf32fbe8ba0c7919ccdc2df4f8fb923804a",
    )

    # Load Google Test
    maybe(
        git_repository,
        name = "gtest",
        remote = "https://github.com/google/googletest.git",
        # 1.10.0
        commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
        shallow_since = "1570114335 -0400"
    )

    allegro()
    curl()
    doxygen()
    sdl2_image()
    sdl2()
    mygui()
    bullet()
    xml2()

    sdl2_image_prebuilt()

    bullet()
    osg()
    physfs()
