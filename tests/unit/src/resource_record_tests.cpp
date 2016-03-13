#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <boost/asio/detail/socket_ops.hpp>

#include "record_data/a_record_data.h"
#include "resource_record.h"

TEST(ResourceRecord, TestToBuffer)
{
	auto aRecordData = std::make_shared<ARecordData>("192.168.1.2");
	ResourceRecord record("foo.com", RecordType::A, RecordClass::INTERNET, 60,
	                      aRecordData);

	std::vector<std::uint8_t> buffer;
	record.toBuffer(buffer);

	ASSERT_EQ(23, buffer.size());

	// Verify domain name
	auto iterator = buffer.cbegin();
	EXPECT_EQ(3, *iterator++);
	EXPECT_EQ('f', *iterator++);
	EXPECT_EQ('o', *iterator++);
	EXPECT_EQ('o', *iterator++);
	EXPECT_EQ(3, *iterator++);
	EXPECT_EQ('c', *iterator++);
	EXPECT_EQ('o', *iterator++);
	EXPECT_EQ('m', *iterator++);
	EXPECT_EQ(0, *iterator++);

	// Verify record type
	int recordType = *iterator++ << 8;
	recordType |= *iterator++;
	recordType = boost::asio::detail::socket_ops::network_to_host_short(
	                 recordType);
	ASSERT_EQ(RecordType::A, recordType);

	// Verify record class
	int recordClass = *iterator++ << 8;
	recordClass |= *iterator++;
	recordClass = boost::asio::detail::socket_ops::network_to_host_short(
	                 recordClass);
	EXPECT_EQ(RecordClass::INTERNET, recordClass);

	// Verify time to live (TTL)
	int timeToLive = *iterator++ << 24;
	timeToLive |= *iterator++ << 16;
	timeToLive |= *iterator++ << 8;
	timeToLive |= *iterator++;
	timeToLive = boost::asio::detail::socket_ops::network_to_host_long(
	                 timeToLive);
	EXPECT_EQ(60, timeToLive);

	// Verify data length
	int dataLength = *iterator++ << 8;
	dataLength |= *iterator++;
	dataLength = boost::asio::detail::socket_ops::network_to_host_short(
	                 dataLength);
	EXPECT_EQ(4, dataLength);

	std::vector<std::uint8_t> dataBuffer(iterator, buffer.cend());
	ARecordData finalData(dataBuffer.cbegin(), dataBuffer.cend());
	EXPECT_EQ(*aRecordData, finalData);
}

TEST(ResourceRecord, TestFromBuffer)
{
	auto aRecordData = std::make_shared<ARecordData>("192.168.1.2");
	ResourceRecord tmpRecord("foo.com", RecordType::A, RecordClass::INTERNET,
	                         60, aRecordData);

	std::vector<std::uint8_t> buffer;
	tmpRecord.toBuffer(buffer);

	ResourceRecord record(buffer);

	EXPECT_EQ("foo.com", record.name());
	EXPECT_EQ(RecordType::A, record.recordType());
	EXPECT_EQ(RecordClass::INTERNET, record.recordClass());
	EXPECT_EQ(60, record.timeToLive());

	ASSERT_EQ(RecordType::A, record.data()->type());
	EXPECT_EQ(*aRecordData,
	          *std::static_pointer_cast<ARecordData>(record.data()));
}
