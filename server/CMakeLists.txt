file(GLOB server_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM server_SOURCES "main.cpp")

add_library(traffic_service_lib ${server_SOURCES})

target_link_libraries(traffic_service_lib messages)
target_link_libraries(traffic_service_lib backend)

target_link_libraries(traffic_service_lib ${ZMQ_LIBRARIES})

target_include_directories(traffic_service_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(traffic_service "main.cpp")
target_link_libraries(traffic_service traffic_service_lib)


install(TARGETS traffic_service RUNTIME DESTINATION bin)
