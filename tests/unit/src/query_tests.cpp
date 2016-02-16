#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <boost/asio/detail/socket_ops.hpp>

#include "query.h"

TEST(Query, TestToBuffer)
{
	Query query("foo.com", RecordType::A, RecordClass::INTERNET);

	std::vector<std::uint8_t> buffer;
	query.toBuffer(buffer);

	ASSERT_EQ(13, buffer.size());

	// Verify domain name
	EXPECT_EQ(3, buffer.at(0));
	EXPECT_EQ('f', buffer.at(1));
	EXPECT_EQ('o', buffer.at(2));
	EXPECT_EQ('o', buffer.at(3));
	EXPECT_EQ(3, buffer.at(4));
	EXPECT_EQ('c', buffer.at(5));
	EXPECT_EQ('o', buffer.at(6));
	EXPECT_EQ('m', buffer.at(7));
	EXPECT_EQ(0, buffer.at(8));

	// Verify record type
	int recordType = boost::asio::detail::socket_ops::network_to_host_short(
	                     (buffer.at(9) << 8) | (buffer.at(10)));
	EXPECT_EQ(RecordType::A, recordType);

	// Verify record class
	int recordClass = boost::asio::detail::socket_ops::network_to_host_short(
	                     (buffer.at(11) << 8) | (buffer.at(12)));
	EXPECT_EQ(RecordClass::INTERNET, recordClass);
}

TEST(Query, TestFromBuffer)
{
	Query tmpQuery("foo.com", RecordType::A, RecordClass::INTERNET);

	std::vector<std::uint8_t> buffer;
	tmpQuery.toBuffer(buffer);

	Query query(buffer);

	EXPECT_EQ("foo.com", query.name());
	EXPECT_EQ(RecordType::A, query.recordType());
	EXPECT_EQ(RecordClass::INTERNET, query.recordClass());
}
