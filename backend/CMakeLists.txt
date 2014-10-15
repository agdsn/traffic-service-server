file(GLOB backend_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(backend STATIC ${backend_SOURCES})
