#!/usr/bin/env python3
# import re
import os
import sys

import CMakeParser


class CMakeConfigureFile:
    def __init__(self, cmake_file):
        self.cmake_file = cmake_file
        self.parser = CMakeParser.CMakeParser(cmake_file)

    def createConfigureFile(self):
        target = self.parser.getTargetName()
        if target is None:
            print(f"Target not found in file: {self.cmake_file}")
            return

        dependencies = self.parser.getDependencies()
        project_type = self.parser.getProjectType()

        if project_type != CMakeParser.CMakeProjectType.LIBRARY:
            print(f"{target} is not a library, not creating configure file")
            return

        target_dir = os.path.dirname(self.cmake_file)
        print(f"Target directory: {target_dir}")

        print(f"Creating configure file for {target}")
        output_path = os.path.join(target_dir, "cmake")
        if not os.path.exists(output_path):
            os.makedirs(output_path)

        out_file = os.path.join(output_path, f"{target}.cmake.in")
        with open(out_file, 'w') as f:
            f.write("@PACKAGE_INIT@\n\n")
            f.write("include(CMakeFindDependencyMacro)\n")
            for dependency in dependencies:
                f.write(f"find_dependency({dependency})\n")

            f.write("\n")
            f.write(f"set(LIB_{target} {target}::{target})\n")

        print(f"Configure file available for viewing: {out_file}")


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        dep_file = CMakeConfigureFile(arg)
        dep_file.createConfigureFile()
