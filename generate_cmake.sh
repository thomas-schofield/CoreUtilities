#!/usr/bin/env bash
cd $(git rev-parse --show-toplevel -q)
if [ $? -ne 0 ]; then
  echo 'Failed to change directory'
fi

cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

