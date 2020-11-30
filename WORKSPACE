workspace(name="KGE")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Foreign CC
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-master",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# Load Allegro
new_git_repository(
    name = "allegro",
    remote = "https://github.com/liballeg/allegro5.git",
    branch = "5.2.6",
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
    tag = "OpenSceneGraph-3.6.5",
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
    build_file = "//.bazel:physfs.bzl"
)
