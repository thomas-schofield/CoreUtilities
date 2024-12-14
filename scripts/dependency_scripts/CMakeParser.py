#!/usr/bin/env python3
import re
import os
import sys
from enum import Enum


class CMakeTargetType(Enum):
    """Enum to describe the different CMake Target Types"""
    INVALID = 1
    BINARY = 2
    LIBRARY = 3


class CMakeParser:
    """A simple CMake parser"""
    def __init__(self, cmake_file: str, use_cache=True):
        if not cmake_file.endswith("CMakeLists.txt") and not os.path.isfile(cmake_file):
            raise IOError(f"{cmake_file} is not a valid file")

        self.cmake_file = cmake_file

        self.cache = use_cache

        # Instance cache variables
        self.target_name = None
        self.target_type = CMakeTargetType.INVALID
        self.target_dependencies = []

    def __str__(self):
        return f"Target: {self.target_name}, Type: {self.target_type}"
    
    def setUseCache(self, state: bool):
        """Toggle class cache usage"""
        self.cache = state

    def getTargetFile(self):
        """Return CMake file for this class instance"""
        return self.cmake_file

    def getTargetDirectory(self):
        """Return directory the CMake file is in"""
        return os.path.dirname(self.cmake_file)

    def getTargetName(self) -> str:
        """Read a CMake file and extract the target name, using cache for repeated accesses if enabled"""
        if self.cache and self.target_name:
            return self.target_name

        with open(self.cmake_file, "r", encoding="utf-8") as f:
            for line in f:
                if line.startswith("set") and "TARGET" in line:
                    pattern = re.escape('set(TARGET "') + "(.*)" + \
                        re.escape('")')
                    p = re.compile(pattern)
                    target_name = p.findall(line)
                    if not target_name:
                        continue

                    # Get first match
                    self.target_name = target_name[0]
                    break

        return self.target_name

    def getTargetType(self) -> CMakeTargetType:
        """Parse the CMake file for the type of target"""
        if self.cache and self.target_type is not CMakeTargetType.INVALID:
            return self.target_type

        with open(self.cmake_file, "r", encoding="utf-8") as f:
            for line in f:
                if re.match("add_library", line):
                    self.target_type = CMakeTargetType.LIBRARY
                    break
                elif re.match("add_executable", line):
                    self.target_type = CMakeTargetType.BINARY
                    break

        return self.target_type

    def getTargetDependencies(self) -> list[str]:
        """Retrieve dependencies from target_link_libraries section of a
            CMakeLists file"""
        if self.cache and len(self.target_dependencies) > 0:
            return self.target_dependencies

        with open(self.cmake_file, "r", encoding="utf8") as f:
            found_dependency_start = False
            for line in f:
                # Starting tag for dependencies
                if line.startswith("set") and ("LINK_LIBS" in line or
                                               "PUBLIC_LINK_LIBS" in line):
                    found_dependency_start = True
                    continue

                # Ending tag for dependencies
                # NOTE: This is expecting the ending parenthesis to be on a
                #       separate line
                if found_dependency_start and ")" in line:
                    found_dependency_start = False
                    break

                # Skip lines until we get to the dependency section
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

            print(f"Running CMakeParser on file: {parser.getTargetFile()}")
            print(f"File exists in directory: {parser.getTargetDirectory()}")

            target = parser.getTargetName()
            target_type = parser.getTargetType()
            dependencies = parser.getTargetDependencies()
            print(f"Found dependencies for {target} (Type: {target_type}): "
                  f"{dependencies}")
        except IOError as e:
            print(f"Encountered error: {e}")
