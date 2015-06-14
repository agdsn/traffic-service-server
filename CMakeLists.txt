cmake_minimum_required (VERSION 2.8.11)
project (traffic-service)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0") # implicit: -g
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native") # implicit: -DNDEBUG -O3 

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options REQUIRED)

find_package(Protobuf REQUIRED)

find_package(Threads)

find_package(PkgConfig)
pkg_check_modules(PC_ZMQ REQUIRED libzmq>=3.0)
find_library(ZMQ_LIBRARIES NAMES zmq libzmq
             HINTS ${PC_ZMQ_LIBDIR} ${PC_ZMQ_LIBRARY_DIRS})

find_library(SQLITE_LIBRARY sqlite3)

set(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE
	${CTEST_SOURCE_DIRECTORY}/test/valgrind/zeromq.supp)
set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")
include(CTest)
enable_testing()

add_subdirectory(messages)
add_subdirectory(backend)
add_subdirectory(server)

add_subdirectory(test)
