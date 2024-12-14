#!/usr/bin/env python3
import re
import os
import sys

import CMakeParser


class CMakeTarget:
    """Retrieve information about a CMake target"""
    def __init__(self, target_file: str, use_cache=True):
        self.cache = use_cache
        self.parser = CMakeParser.CMakeParser(target_file,
                                              use_cache=self.cache)

        self.source_files = []
        self.includes = []

    def __str__(self):
        return f"Target: {self.getTargetName()}, Includes: {self.includes}"

    def setUseCache(self, state: bool) -> None:
        """Define cache usage for class"""
        self.cache = state
        self.parser.setUseCache(state)

    def getTargetName(self) -> str:
        """Retrieve target name via CMakeParser"""
        return self.parser.getTargetName()

    def getTargetType(self) -> CMakeParser.CMakeTargetType:
        """Retrieve target type via CMakeParser"""
        return self.parser.getTargetType()

    def getTargetDependencies(self) -> list[str]:
        """Retrieve target dependencies via CMakeParser"""
        return self.parser.getTargetDependencies()

    def isValidTarget(self) -> bool:
        return self.getTargetType() != CMakeParser.CMakeTargetType.INVALID
    
    def getSourceFiles(self) -> list[str]:
        """Find source files for the CMake target"""
        if not self.isValidTarget():
            return []

        if self.cache and self.source_files:
            return self.source_files

        target_dir = self.parser.getTargetDirectory()
        for (root, _, files) in os.walk(target_dir):
            # Ignore cmake-related directories
            if "cmake" in root.lower():
                continue

            rel_dir = os.path.relpath(root, target_dir)
            for _file in files:
                # Ignore cmake-related files
                if "cmake" in _file.lower():
                    continue

                # Ignore unit test-related files
                if "test" in _file.lower():
                    continue

                selected_file = os.path.join(rel_dir, _file)
                # Get rid of ./
                if selected_file.startswith("./"):
                    selected_file = selected_file[2:]

                if selected_file not in self.source_files:
                    self.source_files.append(selected_file)

        return self.source_files

    def getExternalIncludes(self) -> list[str]:
        """Parses source files and returns external target headers"""
        if not self.isValidTarget():
            return []

        if self.cache and self.includes:
            return self.includes

        # Ensure source files are read
        self.getSourceFiles()

        for _file in self.source_files:
            with open(os.path.join(self.parser.getTargetDirectory(), _file), "r", encoding="utf-8") as f:
                for line in f:
                    if "#include" not in line:
                        continue

                    # remove relative includes
                    if ".." in line:
                        continue

                    pattern = re.escape('"') + "(.*)" + re.escape('"')
                    p = re.compile(pattern)
                    local_include = p.findall(line)
                    if local_include and local_include[0] not in self.source_files:
                        local_include = local_include[0]
                        if local_include not in self.includes:
                            self.includes.append(local_include)
                            continue

                    pattern = re.escape("<") + "(.*)" + re.escape(">")
                    p = re.compile(pattern)
                    system_include = p.findall(line)
                    if system_include and system_include[0] not in self.source_files:
                        system_include = system_include[0]
                        if system_include not in self.includes:
                            self.includes.append(system_include)
                            continue

        self.includes.sort()
        return self.includes


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        try:
            target = CMakeTarget(arg)

            if not target.isValidTarget():
                print(f"No valid target found for: {arg}")
                continue
            
            name = target.getTargetName()
            print(f"Reading data for target: {name}")

            sources = target.getSourceFiles()
            print(f"Source files for {name}: {sources}")

            external_includes = target.getExternalIncludes()
            if external_includes:
                print(f"Found external includes: {external_includes}")
            else:
                print(f"No includes found for: {arg}")
        except Exception as e:
            print(f"Error: {e}")
