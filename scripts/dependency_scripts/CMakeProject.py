#!/usr/bin/env python3
import re
import os
import sys

import CMakeParser


class CMakeProject:
    def __init__(self, project_dir: str):
        if not os.path.isdir(project_dir):
            raise Exception(f"{project_dir} is not a valid directory")

        self.project_dir = project_dir
        self.project_file = os.path.join(self.project_dir, "CMakeLists.txt")

    def isValidProject(self) -> bool:
        try:
            parser = CMakeParser.CMakeParser(self.project_file)
            return parser.getProjectType() == CMakeParser.CMakeProjectType.LIBRARY
        except Exception as e:
            print(e)
            return False

    def getProjectIncludes(self) -> list[str]:
        includes = []

        if not self.isValidProject():
            return includes

        project_files = os.listdir(self.project_dir)
        for entry in project_files:
            project_file = os.path.join(self.project_dir, entry)
            if not os.path.isfile(project_file):
                continue

            if "cmake" in entry.lower():
                continue

            with open(project_file, "r") as f:
                for line in f:
                    if "#include" not in line:
                        continue

                    pattern = re.escape("<") + "(.*)" + re.escape(">")
                    p = re.compile(pattern)
                    system_include = p.findall(line)
                    if system_include and system_include[0] not in project_files: 
                        system_include = system_include[0]
                        if system_include not in includes:
                            includes.append(system_include)
                            continue

                    pattern = re.escape('"') + "(.*)" + re.escape('"')
                    p = re.compile(pattern)
                    local_include = p.findall(line)
                    if local_include and local_include[0] not in project_files:
                        local_include = local_include[0]
                        if local_include not in includes:
                            includes.append(local_include)
                            continue

        if len(includes) > 0:
            includes.sort()
        return includes

    def getDependencies(self):
        raise NotImplementedError


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        project = CMakeProject(arg)

        includes = project.getProjectIncludes()
        if includes:
            print(f"Found includes: {includes}")
        else:
            print(f"No includes found for: {arg}")
