#include <gtest/gtest.h>

#include "record_data/a_record_data.h"

TEST(ARecordData, TestType)
{
	ARecordData data("192.168.1.1");
	EXPECT_EQ(RecordType::A, data.type());
}

TEST(ARecordData, TestToBuffer)
{
	ARecordData data("192.168.1.2");

	std::vector<std::uint8_t> buffer;
	data.toBuffer(buffer);

	ASSERT_EQ(4, buffer.size());

	// Verify IP address octets
	auto iterator = buffer.cbegin();
	EXPECT_EQ(192, *iterator++);
	EXPECT_EQ(168, *iterator++);
	EXPECT_EQ(1, *iterator++);
	EXPECT_EQ(2, *iterator);
}

TEST(ARecordData, TestFromBuffer)
{
	ARecordData tmpData("192.168.1.2");

	std::vector<std::uint8_t> buffer;
	tmpData.toBuffer(buffer);

	ARecordData data(buffer);

	EXPECT_EQ("192.168.1.2", data.ipAddress().to_string());
}
