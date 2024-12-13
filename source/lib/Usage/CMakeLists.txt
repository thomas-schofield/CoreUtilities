set(TARGET "Usage")
set(LIBRARY_TYPE SHARED)

set(SOURCE_FILES
  Usage.C
)

add_library(${TARGET} ${LIBRARY_TYPE} ${SOURCE_FILES})

apply_project_defaults(${TARGET})

# target_include_directories(Usage PUBLIC "")

set(LINK_LIBS
  ${LIB_LoggingServices}
  ${LIB_SafeStrings}
  boost::boost
)

target_link_libraries(${TARGET} PUBLC ${LINK_LIBS})
