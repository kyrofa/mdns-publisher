add_executable(unit-tests
	src/query_tests.cpp
	src/record_data/a_record_data_tests.cpp
	src/record_data/aaaa_record_data_tests.cpp
	src/record_data/record_data_factory_tests.cpp
	src/main.cpp
)

target_link_libraries(unit-tests
	mdns_publisher
	${GMOCK_LIBRARIES}
)

add_test(Foo unit-tests)
