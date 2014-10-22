file(GLOB test_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")


add_subdirectory(gtest)

add_executable(runUnitTests ${test_SOURCES})

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(runUnitTests gtest gtest_main)
target_link_libraries(runUnitTests traffic_service_lib)
target_link_libraries(runUnitTests messages)
target_link_libraries(runUnitTests backend)


add_test(NAME unittests COMMAND runUnitTests)

add_custom_target(test_memcheck COMMAND
	valgrind --leak-check=full
	         --show-reachable=no
		 --track-fds=yes
		 --error-exitcode=1
		 --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/valgrind/zeromq.supp
		 ${CMAKE_CURRENT_BINARY_DIR}/runUnitTests)
