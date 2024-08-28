add_executable(LibraryReader main.cpp)

apply_project_defaults(LibraryReader)

target_include_directories(LibraryReader PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(LibraryReader PRIVATE boost::boost)

# target_include_directories(test-timer PUBLIC ${REPO_ROOT}/include/patterns)