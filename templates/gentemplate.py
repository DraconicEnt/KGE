"""
    template.py
"""

import re
import os
import sys
import shutil

class Application(object):
    value_regex = re.compile("[A-Z]+=\w")
    
    def main(self):
        if (len(sys.argv) <= 3):
            print("Usage: %s <source project> <target dir> VARNAME=VALUE VARNAME=VALUE ..." % sys.argv[0])
            return

        print("*** RUNNING PROJECT GENERATOR")
        if (os.path.isdir(sys.argv[2])):
            print("*** PROJECT FOLDER ALREADY EXISTS")
            return
        if (os.path.isdir(sys.argv[1]) is False):
            print("*** SOURCE PROJECT DIR DOESN'T EXIST")
            return
        
        shutil.copytree(sys.argv[1], os.path.join(sys.argv[2]))
        
        dictionary = { }   
        
        for index, parameter in enumerate(sys.argv):
            if (index < 3):
                continue
            
            if (re.match(self.value_regex, parameter) is None):
                print("Warning: Invalid syntax for parameter '%s' at argv[%u]." % (parameter, index))
            else:
                key, value = tuple(parameter.split("=", 1))
                key = "{{%s}}" % key
                
                dictionary[key] = value
            
        for path, directories, files in os.walk(sys.argv[2]):
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
                    
        print("*** PROJECT GENERATOR COMPLETE")
                            
if __name__ == "__main__":
    Application().main()