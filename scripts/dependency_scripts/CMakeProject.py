#!/usr/bin/env python3
import re
import os
import sys

import CMakeTarget


class CMakeProject:
    """Retrieve information about a set of CMake Targets"""
    def __init__(self, project_dir: str):
        self.project_dir = project_dir

        self.cache = True
        self.project_name = None
        self.project_targets = []
        self.all_deps = []

    def setUseCache(self, state: bool):
        self.cache = state
    
    def getProjectDirectory(self):
        return self.project_dir
    
    def setProjectName(self, project_name: str) -> None:
        self.project_name = project_name
    
    def getProjectName(self):
        return self.project_name

    def getAllTargets(self):
        if self.cache and self.project_targets:
            return self.project_targets

        for (root, _, files) in os.walk(self.project_dir):
            if "CMakeLists.txt" in files:
                try:
                    project_target_file = os.path.join(root, "CMakeLists.txt")
                    project_target = CMakeTarget.CMakeTarget(project_target_file)
                    if not project_target.isValidTarget():
                        continue

                    if project_target not in self.project_targets:
                        self.project_targets.append(project_target)
                except Exception as e:
                    print(f"Caught exception: {e}")

        return self.project_targets

    def getAllDependencies(self):
        """Retrieve dependencies for all project targets, only returning unique results"""
        if self.cache and self.all_deps:
            return self.all_deps

        _targets = self.getAllTargets()

        for target in _targets:
            _deps = target.getTargetDependencies()
            for dep in _deps:
                if dep not in self.all_deps:
                    self.all_deps.append(dep)

        self.all_deps.sort()
        return self.all_deps


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        project = CMakeProject(arg)
        targets = project.getAllTargets()
        for found_target in targets:
            print(f"Resolved target: {found_target}")

        deps = project.getAllDependencies()
        print(f"Dependencies: {deps}")
