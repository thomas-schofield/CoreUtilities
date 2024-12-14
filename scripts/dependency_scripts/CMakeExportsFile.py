#!/usr/bin/env python3
import re
import os
import sys

import CMakeProject


class CMakeExportsFile:
    def __init__(self):
        pass

    def create(self, project: CMakeProject.CMakeProject, ):
        output_dir = project.getProjectDirectory()
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        name = project.getProjectName()
        if not name:
            name = os.path.basename(output_dir)

        output_file = os.path.join(output_dir, "exports.cmake")
        all_targets = project.getAllTargets()
        with open(output_file, "w") as f:
            f.write(f'message(STATUS, "Finding packages for {name}")\n\n')
            for target in all_targets:
                f.write(f"find_package({target.getTargetName()} REQUIRED)\n")

    def getTargetsFromFile(self, in_file: str):
        """Read an export file and retrieve the targets listed in it"""

        if not os.path.isfile(in_file):
            print(f"Cannot read file: {in_file}")
            return []

        targets = []

        with open(in_file, "r") as f:
            for line in f:
                pattern = re.escape("find_package(") + "(.*)" + re.escape(" REQUIRED)")
                p = re.compile(pattern)
                target = p.findall(line)
                if not target:
                    continue

                target = target[0]
                if target not in targets:
                    targets.append(target)

        targets.sort()
        return targets


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        exports_file = CMakeExportsFile()
        found_targets = exports_file.getTargetsFromFile(arg)
        print(f"Found targets: {found_targets}")

        my_project = CMakeProject.CMakeProject("source/lib")
        my_project.setProjectName("CoreUtilities-lib")
        exports_file.create(my_project)
