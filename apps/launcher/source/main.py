"""
    launcher.py
"""

import re
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
        
        try:
            self.process = subprocess.Popen(["./Engine", "-game", "template"], stdout=subprocess.PIPE, stderr=error_log)
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
