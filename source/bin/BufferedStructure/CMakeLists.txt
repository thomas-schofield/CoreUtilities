add_executable(BufferedStructure main.cpp)

apply_project_defaults(BufferedStructure)

target_include_directories(BufferedStructure PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(BufferedStructure PRIVATE boost::boost)

# target_include_directories(test-timer PUBLIC ${REPO_ROOT}/include/patterns)
