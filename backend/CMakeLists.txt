file(GLOB backend_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(backend STATIC ${backend_SOURCES})

target_link_libraries(backend messages)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
					   ${CMAKE_CURRENT_SOURCE_DIR})