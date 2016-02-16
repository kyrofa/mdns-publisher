cmake_minimum_required(VERSION 2.6)
project(mdns-publisher)

find_package(Boost REQUIRED system)

add_definitions("-std=c++11 -Wall -Wextra")

include_directories(
	include
	${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

set(HEADERS
	include/dns_packet.h
	include/mdns_publisher.h
	include/query.h
	include/record_type.h
	include/record_class.h
	include/resource_record.h
)

set(SOURCES
	src/dns_packet.cpp
	src/mdns_publisher.cpp
	src/query.cpp
	src/resource_record.cpp
)

add_library(mdns_publisher
	${HEADERS}
	${SOURCES}
)

target_link_libraries(mdns_publisher
	${Boost_LIBRARIES}
)

add_executable(mdns-publisherd src/main.cpp)

target_link_libraries(mdns-publisherd
	mdns_publisher
)

set(BUILD_TESTS true CACHE BOOL "Whether or not to build the tests")
if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
