# Use options defined in options.cmake to eanble and disable specific compiler/linker flags

# set(CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-14)



# Global project features
add_library(project_features INTERFACE)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE) # Enforce -std=gnu++##
target_compile_features(project_features INTERFACE cxx_std_11)

add_library(project_definitions INTERFACE)

add_library(project_options INTERFACE)

if (ENABLE_COVERAGE)
    target_compile_options(project_options INTERFACE --coverage -O0)
    target_link_libraries(project_options INTERFACE --coverage)
endif()

if (ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
endif()

if (ENABLE_LSAN)
    target_compile_options(project_options INTERFACE -fsanitize=leak)
endif()

if (ENABLE_USAN)
    target_compile_options(project_options INTERFACE -fsanitize=undefined)
endif()

add_library(project_warnings INTERFACE)

if (DISABLE_WARNINGS)
    target_compile_options(project_warnings INTERFACE -w)
endif()

target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Wformat=2)

add_library(project_errors INTERFACE)

target_compile_options(project_errors INTERFACE -Werror)
