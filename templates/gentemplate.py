"""
    template.py
"""

import re
import os
import sys
import shutil
import argparse

class Application(object):
    def main(self):
        """
            Main application entry point.
        """
        workspace_directory = os.environ["BUILD_WORKSPACE_DIRECTORY"]

        parser = argparse.ArgumentParser(description="Generate a new KGE project.")
        parser.add_argument("--template", type=str, required=True, help="The path to the template to use.")
        parser.add_argument("--out", type=str, required=True, help="The path to write the output to.")
        arguments = parser.parse_args()

        if (len(sys.argv) <= 3):
            print("Usage: %s <source project> <target dir> VARNAME=VALUE VARNAME=VALUE ..." % sys.argv[0])
            return

        print("*** RUNNING PROJECT GENERATOR")
        if (os.path.isdir(arguments.out)):
            print("*** PROJECT FOLDER ALREADY EXISTS")
            return
        if (os.path.isdir(arguments.template) is False):
            print("*** SOURCE PROJECT DIR DOESN'T EXIST")
            return

        shutil.copytree(arguments.template, os.path.join(arguments.out))

        # Build replacements
        dictionary = {
            "{BUILD_WORKSPACE_DIRECTORY}": workspace_directory
        }

        for path, directories, files in os.walk(arguments.out):
            for file in files:
                full_path = os.path.join(path, file)

                if (os.path.isfile(full_path) is False):
                    continue

                try:
                    with open(full_path, "r") as handle:
                        payload = handle.read()

                        for sequence in dictionary.keys():
                            payload = payload.replace(sequence, dictionary[sequence])

                    with open(full_path, "w") as handle:
                        handle.write(payload)
                except UnicodeDecodeError as e:
                    continue

        with open(os.path.join(arguments.out, "BUILD"), "w") as handle:
            handle.write("# Empty")

        print("*** PROJECT GENERATOR COMPLETE")

if __name__ == "__main__":
    exit(Application().main())
