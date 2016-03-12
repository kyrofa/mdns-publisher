#include <gtest/gtest.h>

#include "record_type.h"
#include "record_data/a_record_data.h"
#include "record_data/aaaa_record_data.h"
#include "record_data/record_data_factory.h"

TEST(RecordDataFactory, TestARecordData)
{
	ARecordData expected("192.168.1.2");

	std::vector<std::uint8_t> buffer;
	expected.toBuffer(buffer);

	std::shared_ptr<RecordData> data = RecordDataFactory::create(RecordType::A,
	                                                             buffer);

	ASSERT_EQ(RecordType::A, data->type());
	EXPECT_EQ(expected, *std::static_pointer_cast<ARecordData>(data));
}

TEST(RecordDataFactory, TestAAAARecordData)
{
	AAAARecordData expected("1234:5678:9ABC:DEF1:6171:8192:0212:2232");

	std::vector<std::uint8_t> buffer;
	expected.toBuffer(buffer);

	std::shared_ptr<RecordData> data = RecordDataFactory::create(
	                                       RecordType::AAAA, buffer);

	ASSERT_EQ(RecordType::AAAA, data->type());
	EXPECT_EQ(expected, *std::static_pointer_cast<AAAARecordData>(data));
}

TEST(RecordDataFactory, TestInvalidRecordType)
{
	EXPECT_THROW(RecordDataFactory::create(RecordType::INVALID,
	                                       std::vector<std::uint8_t>()),
	             std::out_of_range);
}
