workspace(name="KGE")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Foreign CC
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-master",
   sha256 = "3e6b0691fc57db8217d535393dcc2cf7c1d39fc87e9adb6e7d7bab1483915110",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# Load rules_python
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "rules_python",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
    sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
)

load("@rules_python//python:pip.bzl", "pip_repositories")
pip_repositories()

# Load rules_pkg
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "rules_pkg",
    url = "https://github.com/bazelbuild/rules_pkg/releases/download/0.3.0/rules_pkg-0.3.0.tar.gz",
    sha256 = "6b5969a7acd7b60c02f816773b06fcf32fbe8ba0c7919ccdc2df4f8fb923804a",
)
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
rules_pkg_dependencies()

# Load Google Test
git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest.git",
    # 1.10.0
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    shallow_since = "1570114335 -0400"
)

# Load Allegro
new_git_repository(
    name = "allegro",
    remote = "https://github.com/liballeg/allegro5.git",
    # 5.2.6
    commit = "5e0db6b985eb2714fd276d8808b45f31ea6f52d9",
    shallow_since = "1581300180 -0800",
    build_file = "//.bazel:allegro.bzl"
)

# Load CURL
http_archive(
    name = "curl",
    urls = [
        "https://curl.se/download/curl-7.73.0.tar.gz"
    ],
    sha256 = "ba98332752257b47b9dea6d8c0ad25ec1745c20424f1dd3ff2c99ab59e97cf91",
    build_file = "//.bazel:curl.bzl"
)

# Load SDL2_Image
http_archive(
    name = "sdl2_image",
    urls = [
        "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.tar.gz"
    ],
    build_file = "//.bazel:sdl2_image.bzl"
)

# Load SDL2
http_archive(
    name = "sdl2",
    urls = [
        "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
    ],
    build_file = "//.bazel:sdl2.bzl"
)

# Load MyGUI
new_git_repository(
    name = "mygui",
    remote = "https://github.com/MyGUI/mygui.git",
    tag = "MyGUI3.4.0",
    build_file = "//.bazel:mygui.bzl"
)

# Load Bullet
new_git_repository(
    name = "bullet",
    remote = "https://github.com/bulletphysics/bullet3.git",
    # Tag 3.0.7
    commit = "272c7099d3ba8ac9e8c142e361e8d4cf5d91b8f2",
    shallow_since = "1606325146 -0800",
    build_file = "//.bazel:bullet.bzl"
)

# Load OpenSceneGraph
new_git_repository(
    name = "osg",
    remote = "https://github.com/openscenegraph/OpenSceneGraph.git",
    # Tag 3.6.5
    commit = "a827840baf0786d72e11ac16d5338a4ee25779db",
    shallow_since = "1580468587 +0000",
    build_file = "//.bazel:osg.bzl"
)

# Load ENet
http_archive(
    name = "enet",
    urls = [
        "http://enet.bespin.org/download/enet-1.3.17.tar.gz"
    ],
    sha256 = "a38f0f194555d558533b8b15c0c478e946310022d0ec7b34334e19e4574dcedc",
    build_file = "//.bazel:enet.bzl"
)

# PhysicsFS
http_archive(
    name = "physfs",
    urls = [
        "https://www.icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2"
    ],
    sha256 = "304df76206d633df5360e738b138c94e82ccf086e50ba84f456d3f8432f9f863",
    build_file = "//.bazel:physfs.bzl"
)
