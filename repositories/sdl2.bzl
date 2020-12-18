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
                "SDL2-2.0.12/src/video/dummy/*.h",
                "SDL2-2.0.12/src/video/khronos/vulkan/*.h"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/video/*.h",
                "SDL2-2.0.12/src/video/dummy/*.h"
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
                "SDL2-2.0.12/src/video/dummy/*.c",
                "SDL2-2.0.12/src/video/windows/*.c",
                "SDL2-2.0.12/src/video/windows/*.h",
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/video/*.c",
                "SDL2-2.0.12/src/video/dummy/*.c",
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
                "SDL2-2.0.12/src/joystick/hidapi/SDL_hidapi_ps4.c",
                "SDL2-2.0.12/src/joystick/hidapi/SDL_hidapi_steam.c",
                "SDL2-2.0.12/src/joystick/hidapi/SDL_hidapi_xbox360.c",
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
    name = "power_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/power/*.c",
                "SDL2-2.0.12/src/power/windows/*.c"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/power/*.c",
                "SDL2-2.0.12/src/power/linux/*.c"
            ]
        )
    })
)

filegroup(
    name = "power_headers",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/power/*.h",
                "SDL2-2.0.12/src/power/windows/*.h"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/power/*.h",
                "SDL2-2.0.12/src/power/linux/*.h"
            ]
        )
    })
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
    name = "audio_sources",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/audio/*.c",
                "SDL2-2.0.12/src/audio/winmm/*.c"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/audio/*.c",
                "SDL2-2.0.12/src/audio/alsa/*.c"
            ]
        )
    })
)

filegroup(
    name = "audio_headers",
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(
            include = [
                "SDL2-2.0.12/src/audio/*.h",
                "SDL2-2.0.12/src/audio/winmm/*.h"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/audio/*.h",
                "SDL2-2.0.12/src/audio/alsa/*.h"
            ]
        )
    })
)

filegroup(
    name = "sensor_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/sensor/*.c",
                "SDL2-2.0.12/src/sensor/dummy/*.c"
            ]
        )
)

filegroup(
    name = "sensor_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/sensor/*.h",
                "SDL2-2.0.12/src/sensor/dummy/*.h"
            ]
        )
)

filegroup(
    name = "libm_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/libm/*.c"
            ]
        )
)

filegroup(
    name = "atomic_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/atomic/*.c"
            ]
        )
)

filegroup(
    name = "libm_headers",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/libm/*.h"
            ]
        )
)

filegroup(
    name = "hidapi_sources",
    srcs = glob(
            include = [
                "SDL2-2.0.12/src/hidapi/*.c",
                "SDL2-2.0.12/src/hidapi/windows/*.c",
                "SDL2-2.0.12/src/hidapi/libusb/*.c",
                "SDL2-2.0.12/src/hidapi/libusb/*.cpp"
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
                "SDL2-2.0.12/src/hidapi/libusb/*.h",
                "SDL2-2.0.12/src/hidapi/windows/hid.c"
            ]
        ),

        # Linux
        "//conditions:default": glob(
            include = [
                "SDL2-2.0.12/src/hidapi/hidapi/*.h",
                "SDL2-2.0.12/src/hidapi/libusb/*.h",
                "SDL2-2.0.12/src/hidapi/linux/hid.c"
            ]
        )
    })
)

filegroup(
    name = "cpuinfo_sources",
    srcs = [
        "SDL2-2.0.12/src/cpuinfo/SDL_cpuinfo.c"
    ]
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
    defines = [
        "LIBC=ON",
        "HIDAPI=OFF",
        "DIRECTX=OFF",
        "RENDER_D3D=OFF",
        "VIDEO_VULKAN=OFF",
        "WIN32=1",
        "_WINDOWS=1",
        "SDL2_EXPORTS=1"
    ],
    linkopts = [
        "gdi32.lib",
        "user32.lib",
        "kernel32.lib",
        "ole32.lib",
        "Imm32.lib",
        "odbc32.lib",
        "odbccp32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "winmm.lib",
        # "dxerr8.lib",
        "dxguid.lib",
        "dinput8.lib",
        "imagehlp.lib",
        "oleaut32.lib",
        "shell32.lib",
        "version.lib",
        "uuid.lib"
    ],
    srcs = [
        ":includes",

        ":events",

        ":audio_sources",
        ":audio_headers",

        ":thread_headers",
        ":thread_sources",

        ":yuv2grb_headers",
        ":yuv2grb_sources",

        ":video_headers",
        ":video_sources",

        ":power_sources",
        ":power_headers",

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

        ":libm_headers",
        ":libm_sources",

        ":atomic_sources",

        ":cpuinfo_sources"

        # ":hidapi_sources",
        # ":hidapi_includes"
    ],
    hdrs = [
        ":yuv2grb_headers",
        ":common_headers",
        ":video_headers",
        ":haptic_headers",
        ":dynapi_headers",
        ":core_headers",
        ":power_headers",
        ":libm_headers",
        ":audio_headers",
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
