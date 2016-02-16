#include <gmock/gmock.h>

int main(int argc, char *argv[])
{
	// Initialize Google Mock and Google Test
	testing::InitGoogleMock(&argc, argv);

	// Run all linked tests.
	return RUN_ALL_TESTS();
}
