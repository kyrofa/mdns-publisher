#include "record_data/a_record_data.h"

#include <gtest/gtest.h>

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
	EXPECT_EQ(192, buffer.at(0));
	EXPECT_EQ(168, buffer.at(1));
	EXPECT_EQ(1, buffer.at(2));
	EXPECT_EQ(2, buffer.at(3));
}

TEST(ARecordData, TestFromBuffer)
{
	ARecordData tmpData("192.168.1.2");

	std::vector<std::uint8_t> buffer;
	tmpData.toBuffer(buffer);

	ARecordData data(buffer);

	EXPECT_EQ("192.168.1.2", data.ipAddress().to_string());
}
