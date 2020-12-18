load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def sdl2():
    # Load SDL2
    maybe(
        http_archive,
        name = "sdl2",
        urls = [
            "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
        ],
        sha256 = "349268f695c02efbc9b9148a70b85e58cefbbf704abd3e91be654db7f1e2c863",
        build_file_content = """

filegroup(
    name = "common_headers",
    srcs = glob(
        include = [
            "SDL2-2.0.12/src/*.h"
        ]
    )
)

filegroup(
    name = "common_sources",
    srcs = glob(
        include = [
            "SDL2-2.0.12/src/*.c"
        ]
    )
)

filegroup(
    name = "dynapi_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/dynapi/*.h"
            ]
        )
)

filegroup(
    name = "dynapi_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/dynapi/*.c"
            ]
        )
)

filegroup(
    name = "video_headers",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/video/*.h",
                "SDL2-2.0.12/src/video/khronos/vulkan/*.h"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/video/*.h"
            ]
        )
    })
)

filegroup(
    name = "video_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/video/*.c",
                "SDL2-2.0.12/src/video/windows/*.c",
                "SDL2-2.0.12/src/video/windows/*.h",
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/video/*.c",
                "SDL2-2.0.12/src/video/x11/*.c",
                "SDL2-2.0.12/src/video/x11/*.h",
            ]
        )
    })
)

filegroup(
    name = "events",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/events/*.h",
                "SDL2-2.0.12/src/events/*.c"
            ]
        )
)

filegroup(
    name = "timer_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/timer/*.c",
                "SDL2-2.0.12/src/timer/windows/*.c"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/timer/*.c",
                "SDL2-2.0.12/src/timer/unix/*.c"
            ]
        )
    })
)

filegroup(
    name = "timer_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/timer/*.h"
            ]
        )
)

filegroup(
    name = "thread_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/thread/*.c",
                "SDL2-2.0.12/src/thread/windows/*.c",
                "SDL2-2.0.12/src/thread/windows/*.h",
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/thread/*.c",
                "SDL2-2.0.12/src/thread/pthread/*.c",
                "SDL2-2.0.12/src/thread/pthread/*.h",
            ]
        )
    })
)

filegroup(
    name = "thread_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/thread/*.h",
                "SDL2-2.0.12/src/thread/generic/*.h"
            ]
        )
)

filegroup(
    name = "yuv2grb_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/video/yuv2rgb/*.c"
            ]
        )
)

filegroup(
    name = "yuv2grb_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/video/yuv2rgb/*.h"
            ]
        )
)

filegroup(
    name = "haptic_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/haptic/*.c",
                "SDL2-2.0.12/src/haptic/windows/*.c",
                "SDL2-2.0.12/src/haptic/windows/*.h"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/haptic/*.c",
                "SDL2-2.0.12/src/haptic/linux/*.c"
            ]
        )
    })
)

filegroup(
    name = "haptic_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/haptic/*.h"
            ]
        )
)

filegroup(
    name = "joystick_sources",
    srcs = select({
      "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/joystick/*.c",
                "SDL2-2.0.12/src/joystick/windows/*.c",
                "SDL2-2.0.12/src/joystick/windows/*.h",

                # hidapi
                "SDL2-2.0.12/src/joystick/hidapi/SDL_hidapijoystick.c",
                ]
            ),

      # Linux
      "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/joystick/*.c",
                "SDL2-2.0.12/src/joystick/linux/*.c",
                "SDL2-2.0.12/src/joystick/linux/*.h",

                # hidapi
                "SDL2-2.0.12/src/joystick/hidapi/SDL_hidapijoystick.c",
                ]
            )
    }),


)

filegroup(
    name = "joystick_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/joystick/*.h",
                # hidapi
                "SDL2-2.0.12/src/joystick/hidapi/SDL_hidapijoystick_c.h",
            ]
        )
)

filegroup(
    name = "core_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/core/windows/*.c"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/core/linux/*.c"
            ]
        )
    })
)

filegroup(
    name = "core_headers",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/core/windows/*.h"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/core/linux/*.h"
            ]
        )
    })
)

filegroup(
    name = "sensor_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/sensor/*.c"
            ]
        )
)

filegroup(
    name = "sensor_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/sensor/*.h"
            ]
        )
)

filegroup(
    name = "hidapi_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/hidapi/*.c",
                # "SDL2-2.0.12/src/hidapi/linux/*.cpp"
            ],
            exclude = [
                "SDL2-2.0.12/src/hidapi/SDL_hidapi.c",
            ]
        )
)

filegroup(
    name = "hidapi_includes",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/hidapi/hidapi/*.h",
                "SDL2-2.0.12/src/hidapi/windows/hid.c"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/hidapi/hidapi/*.h",
                "SDL2-2.0.12/src/hidapi/linux/hid.c"
            ]
        )
    })
)

filegroup(
    name = "includes",
    srcs = glob(
        include = [
            "SDL2-2.0.12/include/*.h"
        ]
    )
)

cc_library(
    name = "library",
    srcs = [
        ":includes",

        ":events",

        ":thread_headers",
        ":thread_sources",

        ":yuv2grb_headers",
        ":yuv2grb_sources",

        ":video_headers",
        ":video_sources",

        ":timer_headers",
        ":timer_sources",

        ":common_sources",
        ":common_headers",

        # ":dynapi_sources",
        ":dynapi_headers",

        ":haptic_sources",
        ":haptic_headers",

        ":joystick_headers",
        ":joystick_sources",

        ":core_sources",
        ":core_headers",

        ":sensor_headers",
        ":sensor_sources",

        # ":hidapi_sources",
        ":hidapi_includes"
    ],
    hdrs = [
        ":yuv2grb_headers",
        ":common_headers",
        ":video_headers",
        ":haptic_headers",
        ":dynapi_headers",
        ":core_headers",
        ":joystick_headers",
        ":sensor_headers",
        ":hidapi_includes",
        ":includes"
    ],
    includes = [
        "SDL2-2.0.12/include",
        "SDL2-2.0.12/src/joystick",
        "SDL2-2.0.12/src/hidapi/hidapi",
    ],
    visibility = ["//visibility:public"]
)
    """
    )
