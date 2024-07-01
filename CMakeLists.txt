cmake_minimum_required(VERSION 3.11)

project(CoreUtilities LANGUAGES CXX)

set(REPO_ROOT ${PROJECT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${REPO_ROOT}/cmake)

include(options)
include(project)
include(macros)
include(dependencies)

add_subdirectory(source)

if (ENABLE_TESTING)
    include(CTest)
    enable_testing()

    add_subdirectory(test)
endif()
