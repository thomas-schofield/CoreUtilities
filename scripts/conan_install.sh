#!/usr/bin/env bash

cd $(git rev-parse --show-toplevel -q)

if [ $? -ne 0  ]; then
  echo 'Failed to change directory'
fi

conan install . --output-folder=build --build=missing
