list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Gmock REQUIRED)
include_directories(
	${GTEST_INCLUDE_DIRS}
	${GMOCK_INCLUDE_DIRS}
)

add_subdirectory(unit)
