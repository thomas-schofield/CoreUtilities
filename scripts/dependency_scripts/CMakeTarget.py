#!/usr/bin/env python3
import re
import os
import sys

import CMakeParser


class CMakeTarget:
    """Retrieve information about a CMake target"""
    def __init__(self, target_file: str):
        self.parser = CMakeParser.CMakeParser(target_file)
        self.cache = True

        self.includes = []

    def __str__(self):
        if self.isValidTarget():
            return self.getTargetName()
        return f"Invalid target: {self.parser.getTargetFile()}"

    def setUseCache(self, state: bool) -> None:
        self.cache = state
        self.parser.setUseCache(state)

    def getTargetName(self) -> str:
        return self.parser.getTargetName()

    def getTargetType(self) -> CMakeParser.CMakeTargetType:
        return self.parser.getTargetType()

    def getTargetDependencies(self) -> list[str]:
        return self.parser.getTargetDependencies()

    def isValidTarget(self) -> bool:
        try:
            return self.getTargetType() == CMakeParser.CMakeTargetType.LIBRARY
        except Exception as e:
            print(e)
            return False

    def getTargetIncludes(self) -> list[str]:
        if not self.isValidTarget():
            return []
        
        if self.cache and self.includes:
            return self.includes

        target_files = os.listdir(self.parser.getTargetDirectory())
        for entry in target_files:
            target_file = os.path.join(self.parser.getTargetDirectory(), entry)
            if not os.path.isfile(target_file):
                continue

            if "cmake" in entry.lower():
                continue

            with open(target_file, "r") as f:
                for line in f:
                    if "#include" not in line:
                        continue

                    pattern = re.escape("<") + "(.*)" + re.escape(">")
                    p = re.compile(pattern)
                    system_include = p.findall(line)
                    if system_include and system_include[0] not in target_files: 
                        system_include = system_include[0]
                        if system_include not in self.includes:
                            self.includes.append(system_include)
                            continue

                    pattern = re.escape('"') + "(.*)" + re.escape('"')
                    p = re.compile(pattern)
                    local_include = p.findall(line)
                    if local_include and local_include[0] not in target_files:
                        local_include = local_include[0]
                        if local_include not in self.includes:
                            self.includes.append(local_include)
                            continue

        self.includes.sort()
        return self.includes

    def resolveDependencies(self, external_projects: list[str]) -> list[str]:
        includes = self.getTargetIncludes()

        raise NotImplementedError


if __name__ == "__main__":
    external_dependency_file = sys.argv[1]
    external_dependency_file_full_path = os.path.join(os.getcwd(), external_dependency_file)

    for arg in sys.argv[2:]:
        project = CMakeTarget(arg)

        includes = project.getTargetIncludes()
        if includes:
            print(f"Found includes: {includes}")
        else:
            print(f"No includes found for: {arg}")
        
        # external_dependencies = project.getAllExternalTargets(external_dependency_file_full_path)
        # print(f"Retrieved external dependencies: {external_dependencies}")
