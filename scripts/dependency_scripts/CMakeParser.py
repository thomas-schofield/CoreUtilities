#!/usr/bin/env python3
import re
import os
import sys
from enum import Enum


class CMakeTargetType(Enum):
    INVALID = 1
    BINARY = 2
    LIBRARY = 3


class CMakeParser:
    """A CMake parser"""
    def __init__(self, cmake_file: str):
        if not cmake_file.endswith("CMakeLists.txt") and not os.path.isfile(cmake_file):
            raise IOError(f"{cmake_file} is not a valid file")

        self.cmake_file = cmake_file

        self.cache = True

        self.target_name = None
        self.target_type = CMakeTargetType.INVALID
        self.target_dependencies = []

    def setUseCache(self, state: bool):
        """In order to improve data-reaccess, we can cache results"""
        self.cache = state

    def getTargetFile(self):
        return self.cmake_file
    
    def getTargetDirectory(self):
        return os.path.dirname(self.cmake_file)

    def getTargetName(self) -> str:
        if self.cache and self.target_name:
            return self.target_name

        with open(self.cmake_file, "r") as f:
            for line in f:
                if line.startswith("set") and "TARGET" in line:
                    pattern = re.escape('set(TARGET "') + "(.*)" + re.escape('")')
                    p = re.compile(pattern)
                    target_name = p.findall(line)
                    if not target_name:
                        continue

                    # Get first match
                    self.target_name = target_name[0]
                    break

        return self.target_name

    def getTargetType(self) -> CMakeTargetType:
        if self.cache and self.target_type is not CMakeTargetType.INVALID:
            return self.target_type

        with open(self.cmake_file, "r") as f:
            for line in f:
                if re.match("add_library", line):
                    self.target_type = CMakeTargetType.LIBRARY
                    break
                elif re.match("add_executable", line):
                    self.target_type = CMakeTargetType.BINARY
                    break
        
        return self.target_type

    def getTargetDependencies(self) -> list[str]:
        """Retrieve dependencies from a CMakeLists file"""
        if self.cache and len(self.target_dependencies) > 0:
            return self.target_dependencies

        with open(self.cmake_file, "r") as f:
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

                if dependency not in self.target_dependencies:
                    self.target_dependencies.append(dependency)

        self.target_dependencies.sort()
        return self.target_dependencies


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        try:
            parser = CMakeParser(arg)
            print(f"Target Directory: {parser.getTargetDirectory()}")
            print(f"Parsing target file: {parser.getTargetFile()}")
            target = parser.getTargetName()
            target_type = parser.getTargetType()
            dependencies = parser.getTargetDependencies()
            print(f"Found dependencies for {target} (Type: {target_type}): {dependencies}")
        except IOError as e:
            print(f"Encountered error: {e}")
