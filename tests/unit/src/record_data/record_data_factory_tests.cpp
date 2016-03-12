#include "record_data/record_data_factory.h"

#include <gtest/gtest.h>

//TEST(Query, TestFromBuffer)
//{
//	Query tmpQuery("foo.com", RecordType::A, RecordClass::INTERNET);

//	std::vector<std::uint8_t> buffer;
//	tmpQuery.toBuffer(buffer);

//	Query query(buffer);

//	EXPECT_EQ("foo.com", query.name());
//	EXPECT_EQ(RecordType::A, query.recordType());
//	EXPECT_EQ(RecordClass::INTERNET, query.recordClass());
//}
