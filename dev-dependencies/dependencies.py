#!/usr/bin/python
"""
    dependencies.py
"""

import os
import sys
import shutil
import urllib
import zipfile
import tarfile
import platform

class Application(object):
    IRRLICHT_VERSION = "1.8.4"
    FREETYPE_VERSION = "2.7"
    CEGUI_VERSION ="0.8.7"

    def install_ubuntu(self):
        print("Installing for Ubuntu ...")
        os.system("sudo apt-get install -y libenet-dev libbullet-dev libphysfs-dev libcurl4-openssl-dev git libtinyxml2-dev libfreeimage-dev cmake unzip mercurial")

    linux_install_handlers = {
        "LinuxMint": install_ubuntu,
        "Ubuntu": install_ubuntu,
    }

    def download_compile_dependencies(self):
        """
            Platform independent dependency downloader.
        """
        if os.path.exists("downloaded-deps") is False:
            os.mkdir("downloaded-deps")

        print("Downloading compile dependencies ...")
        if os.path.exists("downloaded-deps/irrlicht-%s.zip" % self.IRRLICHT_VERSION) is False:
            print("Downloading Irrlicht ...")
            urllib.urlretrieve("http://downloads.sourceforge.net/irrlicht/irrlicht-%s.zip" % self.IRRLICHT_VERSION, "downloaded-deps/irrlicht-%s.zip" % self.IRRLICHT_VERSION)
            handle = zipfile.ZipFile("downloaded-deps/irrlicht-%s.zip" % self.IRRLICHT_VERSION)
            handle.extractall("downloaded-deps")
            handle.close()

        if os.path.exists("downloaded-deps/freetype-%s.tar.gz" % self.FREETYPE_VERSION) is False:
            print("Downloading FreeType ...")
            urllib.urlretrieve("http://download.savannah.gnu.org/releases/freetype/freetype-%s.tar.gz" % self.FREETYPE_VERSION, "downloaded-deps/freetype-%s.tar.gz" % self.FREETYPE_VERSION)
            handle = tarfile.open("downloaded-deps/freetype-%s.tar.gz" % self.FREETYPE_VERSION)
            handle.extractall("downloaded-deps")
            handle.close()

        if os.path.exists("downloaded-deps/cegui-%s.zip" % self.CEGUI_VERSION) is False:
            print("Downloading CEGUI ...")
            urllib.urlretrieve("https://bitbucket.org/cegui/cegui/downloads/cegui-%s.zip" % self.CEGUI_VERSION, "downloaded-deps/cegui-%s.zip" % self.CEGUI_VERSION)
            handle = zipfile.ZipFile("downloaded-deps/cegui-%s.zip" % self.CEGUI_VERSION)
            handle.extractall("downloaded-deps")
            handle.close()

        self.setup_irrlicht()
        self.setup_dependencies()
        self.compile_dependencies()

    def compile_dependencies(self):
        if "linux" in sys.platform:
            current_cwd = os.getcwd()

            os.chdir("downloaded-deps/freetype-%s" % self.FREETYPE_VERSION)
            os.system("make -j8 && sudo make install")
            os.chdir(current_cwd)

            os.chdir("downloaded-deps/cegui-%s" % self.CEGUI_VERSION)
            os.system("make -j8 && sudo make install")
            os.chdir(current_cwd)

            os.chdir("downloaded-deps/irrlicht-%s" % self.IRRLICHT_VERSION)
            os.system("make -j8 && sudo make install")
            os.chdir(current_cwd)

    def setup_irrlicht(self):
        """
            Platform independent setup routine for Irrlicht.
        """

        irrlicht_config_file = "downloaded-deps/irrlicht-%s/include/IrrCompileConfig.h" % self.IRRLICHT_VERSION
        with open(irrlicht_config_file, "r") as handle:
            irrlicht_config = handle.read()

        disabled_irrlicht_features = """
// Disable all unwanted/unused features.
#define NO_IRR_COMPILE_WITH_SOFTWARE_ 1
#define NO_IRR_COMPILE_WITH_BURNINGSVIDEO_ 1
#define NO_IRR_COMPILE_WITH_X11_ 1
#define NO_IRR_COMPILE_WITH_GUI_
#define NO_IRR_COMPILE_WITH_IRR_SCENE_LOADER_
#define NO_IRR_COMPILE_WITH_OGRE_LOADER_

// physfs will provide our archive support.
#define NO__IRR_COMPILE_WITH_ZIP_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_PAK_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_NPK_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_TAR_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_WAD_ARCHIVE_LOADER_

        """

        irrlicht_config = irrlicht_config.replace("//#define _IRR_COMPILE_WITH_SDL_DEVICE_", "#define _IRR_COMPILE_WITH_SDL_DEVICE_")
        irrlicht_config = disabled_irrlicht_features + irrlicht_config

        with open(irrlicht_config_file, "w") as handle:
            handle.write(irrlicht_config)

    def setup_dependencies(self):
        """
            Initiates the CMake script on all dependencies.
        """

        current_cwd = os.getcwd()

        os.chdir("downloaded-deps/freetype-%s" % self.FREETYPE_VERSION)
        os.system("cmake .")
        os.chdir(current_cwd)

        os.chdir("downloaded-deps/cegui-%s" % self.CEGUI_VERSION)
        os.system("cmake . -DCEGUI_BUILD_PYTHON_MODULES=off -DCEGUI_BUILD_LUA_MODULE=off -DCEGUI_BUILD_RENDERER_IRRLICHT=on -DCEGUI_BUILD_IMAGECODEC_SDL2=on")
        os.chdir(current_cwd)

        os.chdir("downloaded-deps/irrlicht-%s" % self.IRRLICHT_VERSION)
        os.system("cmake . -DWANT_TESTS:string=off -DWANT_EXAMPLES:string=off -DWANT_GLES2=off -DWANT_GLES3=off -DWANT_OPENGL=off -DWANT_SHADERS_GL=off -DWANT_FONT=off -DWANT_AUDIO=off -DWANT_IMAGE=off -DWANT_TTF=off -DWANT_COLOR=off -DWANT_VIDEO=off")
        os.chdir(current_cwd)

    def main(self):
        """
            Main application entry point.
        """

        if "linux" in sys.platform:
            distribution, version, codename = platform.dist()

            if distribution in self.linux_install_handlers:
                self.linux_install_handlers[distribution](self)
                self.download_compile_dependencies()
            else:
                print("Unsupported Linux distribution: %s" % distribution)
                return
        else:
            print("Your platform is unsupported.")
            return

if __name__ == "__main__":
    Application().main()
