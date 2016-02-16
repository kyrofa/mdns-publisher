add_executable(unit-tests
	src/query_tests.cpp
	src/main.cpp
)

target_link_libraries(unit-tests
	mdns_publisher
	${GMOCK_LIBRARIES}
)

add_test(Foo unit-tests)
