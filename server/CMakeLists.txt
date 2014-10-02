file(GLOB server_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(traffic_service ${server_SOURCES})

target_link_libraries(traffic_service messages)
target_link_libraries(traffic_service backend)

target_link_libraries(traffic_service ${ZMQ_LIBRARIES})

install(TARGETS traffic_service RUNTIME DESTINATION bin)
