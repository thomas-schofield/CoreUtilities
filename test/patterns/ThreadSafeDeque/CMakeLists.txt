add_executable(test-ThreadSafeDeque test_ThreadSafeDeque.cpp)
apply_project_defaults(test-ThreadSafeDeque)

target_include_directories(test-ThreadSafeDeque PUBLIC ${REPO_ROOT}/include/patterns)
