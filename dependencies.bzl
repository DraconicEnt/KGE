load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

# Load all external repositories - these are split up for readability
load("@third_party//:enet.bzl", "enet")
load("@third_party//:osg.bzl", "osg")
load("@third_party//:tiff.bzl", "tiff")
load("@third_party//:zlib.bzl", "zlib")
load("@third_party//:png.bzl", "png")
load("@third_party//:freetype.bzl", "freetype")
load("@third_party//:allegro.bzl", "allegro")
load("@third_party//:curl.bzl", "curl")
load("@third_party//:sdl2_image.bzl", "sdl2_image")
load("@third_party//:sdl2_image_prebuilt.bzl", "sdl2_image_prebuilt")
# load("@third_party//:sdl2.bzl", "sdl2")
load("@third_party//:mygui.bzl", "mygui")
load("@third_party//:bullet.bzl", "bullet")
load("@third_party//:physfs.bzl", "physfs")
load("@third_party//:xml2.bzl", "xml2")
load("@third_party//:iconv.bzl", "iconv")
load("@third_party//:lzma.bzl", "lzma")
load("@third_party//:doxygen.bzl", "doxygen")
load("@third_party//:graphviz.bzl", "graphviz")
load("@third_party//:harfbuzz.bzl", "harfbuzz")
load("@third_party//:irrlicht.bzl", "irrlicht")
load("@third_party//:jpeg.bzl", "jpeg")
load("@third_party//:bzip2.bzl", "bzip2")


def dependencies():
    rules_foreign_cc_dependencies()
    rules_pkg_dependencies()

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


    allegro()
    curl()
    doxygen()
    #sdl2_image()
    # sdl2()
    mygui()
    bullet()
    xml2()

    # sdl2_image_prebuilt()

    bullet()
    osg()
    physfs()

def generate_dependencies(target, dependencies, name):
    for index, dependency in enumerate(dependencies):
        rule_name = dependency.replace(".", "_")

        native.genrule(
            name = rule_name,
            srcs = [
                target
            ],
            cmd = "\n".join([
                "LOCATIONS=\"$(locations %s)\"" % target,
                "IFS=' ' read -ra LOCATIONS <<< $$LOCATIONS",

                # Search for our copy target
                "TARGET_BASENAME=$$(basename \"$@\")",
                "for LOCATION in $${LOCATIONS[@]}; do",
                    "LOCATION_BASENAME=$$(basename $$LOCATION)",

                    "if [ \"$$LOCATION_BASENAME\" == \"$$TARGET_BASENAME\" ]; then",
                        "cp $$LOCATION \"$@\"",
                        "exit 0",
                    "fi",
                "done",

                "echo !!! COULD NOT FIND $@",
                "echo SEARCH LIST: ",
                "echo $${LOCATIONS[@]}",
                "exit 1"
            ]),
            outs = [
                dependency
            ]
        )

    native.filegroup(
        name = name,
        srcs = dependencies
    )
