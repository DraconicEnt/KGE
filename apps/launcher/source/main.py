"""
    launcher.py
"""

import re
import os
import sys
from datetime import datetime
import subprocess

class Application(object):
    type_pattern = re.compile("\(\S+\)")
    console_pattern = re.compile("\(\S+\) \S+")
    component_pattern = re.compile("\S+:")
    
    process = None
    
    def main(self):
        # Attempt to open the logfile
        error_log = open("errorLog.txt", "w")
        info_log = open("infoLog.txt", "w")
        
        # Write the log header
        system_name, node_name, release, version, machine = os.uname()
        info_log.write("Begin System Info--------------------------\n")
        info_log.write("System Name: %s\n" % system_name)
        info_log.write("Release: %s\n" % release)
        info_log.write("Machine Type: %s\n" % machine)
        
        parameter_string = "./Engine"
        for index in range(1, len(sys.argv)):
            parameter_string = "%s %s" % (parameter_string, sys.argv[index])
        info_log.write("Subprocess Parameters: %s\n" % parameter_string)
        
        info_log.write("\n")
        info_log.write("Begin Runtime Log--------------------------\n")
        
        try:
            arguments = ["./Engine"] + sys.argv
            
            self.process = subprocess.Popen(arguments, stdout=subprocess.PIPE, stderr=error_log)
            for stdout_line in iter(self.process.stdout.readline, b''):
                stdout_line = stdout_line.rstrip()
                
                if (re.match(self.console_pattern, stdout_line)):
                    message_type = re.search(self.type_pattern, stdout_line).group(0).strip("()")
                    message_content = re.split(self.type_pattern, stdout_line)[1].lstrip()
                    
                    if (re.match(self.component_pattern, message_content)):
                        component_name = re.search(self.component_pattern, message_content).group(0).rstrip(":")
                        message_content = re.split(self.component_pattern, message_content)[1].lstrip()
                        info_log.write("(%s at %s from %s): %s\n" % (message_type, datetime.now(), component_name, message_content))
                    else:
                        info_log.write("(%s at %s): %s\n" % (message_type, datetime.now(), message_content))
                    
            error_log.close()
            info_log.close()
        except OSError:
            print("FATAL: Unable to open engine executable.")
        
if __name__ == "__main__":
    Application().main()
