message(STATUS "Finding core dependencies for CoreUtilities-lib")

# Only list core libraries below
find_package(LoggingServices REQUIRED)
find_package(SafeStrings REQUIRED)
