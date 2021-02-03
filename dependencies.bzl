load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

# Load all external repositories - these are split up for readability
load("@rules_third_party//libraries:enet.bzl", "enet")
load("@rules_third_party//libraries:osg.bzl", "osg")
load("@rules_third_party//libraries:tiff.bzl", "tiff")
load("@rules_third_party//libraries:zlib.bzl", "zlib")
load("@rules_third_party//libraries:png.bzl", "png")
load("@rules_third_party//libraries:freetype.bzl", "freetype")
load("@rules_third_party//libraries:allegro.bzl", "allegro")
load("@rules_third_party//libraries:curl.bzl", "curl")
#load("@rules_third_party//libraries:sdl2_image.bzl", "sdl2_image")
#load("@rules_third_party//libraries:sdl2_image_prebuilt.bzl", "sdl2_image_prebuilt")
# load("@rules_third_party//:sdl2.bzl", "sdl2")
load("@rules_third_party//libraries:mygui.bzl", "mygui")
load("@rules_third_party//libraries:bullet.bzl", "bullet")
load("@rules_third_party//libraries:physfs.bzl", "physfs")
load("@rules_third_party//libraries:xml2.bzl", "xml2")
load("@rules_third_party//libraries:iconv.bzl", "iconv")
load("@rules_third_party//libraries:lzma.bzl", "lzma")
load("@rules_third_party//applications:doxygen.bzl", "doxygen")
load("@rules_third_party//applications:graphviz.bzl", "graphviz")
load("@rules_third_party//libraries:harfbuzz.bzl", "harfbuzz")
load("@rules_third_party//libraries:irrlicht.bzl", "irrlicht")
load("@rules_third_party//libraries:jpeg.bzl", "jpeg")
load("@rules_third_party//libraries:bzip2.bzl", "bzip2")


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
