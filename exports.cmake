message(STATUS, "Exports for External Project")

# List all core libraries needed here
find_package(LoggingServices REQUIRED)
find_package(SafeStrings REQUIRED)
find_package(AsioMulticast REQUIRED)
