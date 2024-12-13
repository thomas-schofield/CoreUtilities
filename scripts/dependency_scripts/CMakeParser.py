#!/usr/bin/env python3
import re
import os
import sys
from enum import Enum


class CMakeProjectType(Enum):
    INVALID = 1
    BINARY = 2
    LIBRARY = 3


class CMakeParser:
    """A CMake parser"""
    def __init__(self, file: str):
        if not file.endswith("CMakeLists.txt") and not os.path.isfile(file):
            raise IOError(f"{file} is not a valid file")

        self.file = file

    def getTargetName(self) -> str:
        target = None

        with open(self.file, "r") as f:
            for line in f:
                if line.startswith("set") and "TARGET" in line:
                    pattern = re.escape('set(TARGET "') + "(.*)" + re.escape('")')
                    p = re.compile(pattern)
                    target_name = p.findall(line)
                    if not target_name:
                        continue

                    # Get first match
                    target_name = target_name[0]
                    target = target_name
                    break

        return target

    def getProjectType(self) -> CMakeProjectType:
        with open(self.file, "r") as f:
            for line in f:
                if re.match("add_library", line):
                    return CMakeProjectType.LIBRARY
                elif re.match("add_executable", line):
                    return CMakeProjectType.BINARY
                
        return CMakeProjectType.INVALID

    def getDependencies(self) -> list[str]:
        """Retrieve dependencies from a CMakeLists file"""
        dependencies = []
        with open(self.file, "r") as f:
            found_dependency_start = False
            for line in f:
                # Starting tag for dependencies
                if line.startswith("set") and ("LINK_LIBS" in line or "PUBLIC_LINK_LIBS" in line):
                    found_dependency_start = True
                    continue

                # Ending tag for dependencies
                if found_dependency_start and ")" in line:
                    found_dependency_start = False
                    break

                if not found_dependency_start:
                    continue

                # We are now parsing dependencies inside of the block
                pattern = re.escape("${LIB_") + "(.*)" + re.escape('}')
                p = re.compile(pattern)
                dependency = p.findall(line)

                if not dependency:
                    continue

                # Get first match
                dependency = dependency[0]

                if dependency not in dependencies:
                    dependencies.append(dependency)

        dependencies.sort()
        return dependencies


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        try:
            parser = CMakeParser(arg)
            target = parser.getTargetName()
            project_type = parser.getProjectType()
            dependencies = parser.getDependencies()
            print(f"Found dependencies for {target} (Type: {project_type}): {dependencies}")
        except IOError as e:
            print(f"Encountered error: {e}")
