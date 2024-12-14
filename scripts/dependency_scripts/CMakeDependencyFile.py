#!/usr/bin/env python3
import re
import os
import sys

import CMakeProject
import CMakeExportsFile


class CMakeDependencyFile:
    def __init__(self, in_project: CMakeProject.CMakeProject):
        self.project = in_project

    def create(self, external_exports_files: list[str]):
        available_external_targets = self._getExternalExports(external_exports_files)
        print(f"Available external targets: {available_external_targets}")
        used_dependencies = self.project.getAllDependencies()
        print(f"Used dependencies: {used_dependencies}")

        targets_for_dependency_file = []
        for dependency in used_dependencies:
            if dependency in available_external_targets:
                targets_for_dependency_file.append(dependency)
        output_dir = self.project.getProjectDirectory()
        output_file = os.path.join(output_dir, "dependencies.cmake")
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(f'message(STATUS "Finding core dependencies for {self.project.getProjectName()}")\n\n')
            f.write("# Only list core libraries below\n")
            for target in targets_for_dependency_file:
                f.write(f"find_package({target} REQUIRED)\n")

    def _getExternalExports(self, external_exports_files: list[str]):
        available_external_targets = []
        for entry in external_exports_files:
            if os.path.isfile(entry):
                exports_file = CMakeExportsFile.CMakeExportsFile()
                external_targets = exports_file.getTargetsFromFile(entry)
                for target in external_targets:
                    if target not in available_external_targets:
                        available_external_targets.append(target)

        available_external_targets.sort()
        return available_external_targets


if __name__ == "__main__":
    project_name = sys.argv[1]
    project_dir = sys.argv[2]
    external_targets_files = [sys.argv[3]]
    project = CMakeProject.CMakeProject(project_dir)
    project.setProjectName(project_name)

    dependency_file = CMakeDependencyFile(project)
    dependency_file.create(external_targets_files)
