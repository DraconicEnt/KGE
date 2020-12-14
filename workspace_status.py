"""
    Tool used to generate workspace status strings.
"""

import os
import subprocess

class Application:
    def main(self):
        # FIXME: Is there git python bindings?
        commit_hash = subprocess.check_output(["git", "rev-parse", "HEAD"], shell=False).decode("utf-8").strip().rstrip()
        print("STABLE_COMMIT_HASH %s" % commit_hash)

if __name__ == "__main__":
    Application().main()
