add_executable(test-timer test_timer.cpp)

apply_project_defaults(test-timer)

target_include_directories(test-timer PUBLIC ${REPO_ROOT}/include/patterns)
