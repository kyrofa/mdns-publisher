#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>

#include "record_data/aaaa_record_data.h"

TEST(AAAARecordData, TestType)
{
	AAAARecordData data("1234:5678:9ABC:DEF1:6171:8192:0212:2232");
	EXPECT_EQ(RecordType::AAAA, data.type());
}

TEST(AAAARecordData, TestSize)
{
	AAAARecordData data("1234:5678:9ABC:DEF1:6171:8192:0212:2232");
	EXPECT_EQ(16, data.size());
}

TEST(AAAARecordData, TestToBuffer)
{
	AAAARecordData data("1234:5678:9ABC:DEF1:6171:8192:0212:2232");

	std::vector<std::uint8_t> buffer;
	data.toBuffer(buffer);

	ASSERT_EQ(16, buffer.size());

	// Verify IP address octets
	auto iterator = buffer.cbegin();
	EXPECT_EQ(0x12, *iterator++);
	EXPECT_EQ(0x34, *iterator++);
	EXPECT_EQ(0x56, *iterator++);
	EXPECT_EQ(0x78, *iterator++);
	EXPECT_EQ(0x9A, *iterator++);
	EXPECT_EQ(0xBC, *iterator++);
	EXPECT_EQ(0xDE, *iterator++);
	EXPECT_EQ(0xF1, *iterator++);
	EXPECT_EQ(0x61, *iterator++);
	EXPECT_EQ(0x71, *iterator++);
	EXPECT_EQ(0x81, *iterator++);
	EXPECT_EQ(0x92, *iterator++);
	EXPECT_EQ(0x02, *iterator++);
	EXPECT_EQ(0x12, *iterator++);
	EXPECT_EQ(0x22, *iterator++);
	EXPECT_EQ(0x32, *iterator);
}

TEST(AAAARecordData, TestFromBuffer)
{
	const std::string full("1234:5678:9ABC:DEF1:6171:8192:0212:2232");
	const std::string shortened("1234:5678:9ABC:DEF1:6171:8192:212:2232");

	AAAARecordData tmpData(full);

	std::vector<std::uint8_t> buffer;
	tmpData.toBuffer(buffer);

	AAAARecordData data(buffer.cbegin(), buffer.cend());

	const std::string actual = boost::to_upper_copy(data.ipAddress().to_string());

	if (actual != full && actual != shortened)
	{
		FAIL() << "Expected IP address to be either " << full << " or "
		       << shortened << " (was: " << actual << ")";
	}
}
