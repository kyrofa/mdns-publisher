#include <cstdint>
#include <vector>
#include <memory>

#include <gtest/gtest.h>

#include <boost/asio/detail/socket_ops.hpp>

#include "record_data/a_record_data.h"
#include "dns_packet.h"

class DnsPacketTest: public ::testing::Test
{
	protected:
		virtual void SetUp()
		{
			query = std::make_shared<Query>("foo.com", RecordType::A, RecordClass::INTERNET);
			std::vector<Query> questions{*query};

			std::shared_ptr<ARecordData> answerRecordData = std::make_shared<ARecordData>("192.168.1.1");
			answerRecord = std::make_shared<ResourceRecord>(
			                   "foo.com", RecordType::A, RecordClass::INTERNET,
			                   60, answerRecordData);
			std::vector<ResourceRecord> answerRecords{*answerRecord};

			std::shared_ptr<ARecordData> authorityRecordData = std::make_shared<ARecordData>("192.168.1.2");
			authorityRecord = std::make_shared<ResourceRecord>(
			                      "foo.com", RecordType::A, RecordClass::INTERNET,
			                      60, authorityRecordData);
			std::vector<ResourceRecord> authorityRecords{*authorityRecord};

			std::shared_ptr<ARecordData> additionalRecordDaa = std::make_shared<ARecordData>("192.168.1.3");
			additionalRecord = std::make_shared<ResourceRecord>(
			                       "foo.com", RecordType::A, RecordClass::INTERNET,
			                       60, additionalRecordDaa);
			std::vector<ResourceRecord> additionalRecords{*additionalRecord};

			packet = std::make_shared<DnsPacket>(
			             1, DnsPacket::QueryResponse::RESPONSE,
			             DnsPacket::Opcode::OPCODE_UPDATE,
			             DnsPacket::AuthoritativeAnswer::AUTHORITATIVE,
			             DnsPacket::Truncated::NOT_TRUNCATED,
			             DnsPacket::RecursionDesired::RECURSION_NOT_DESIRED,
			             DnsPacket::RecursionAvailable::RECURSION_NOT_AVAILABLE,
			             DnsPacket::AuthenticatedData::AUTHENTICATED,
			             DnsPacket::CheckingDisabled::CHECKING_DISABLED,
			             DnsPacket::ReturnCode::RETURN_NO_ERROR,
			             questions, answerRecords, authorityRecords,
			             additionalRecords);
		}

	protected:
		std::shared_ptr<Query> query;
		std::shared_ptr<ResourceRecord> answerRecord;
		std::shared_ptr<ResourceRecord> authorityRecord;
		std::shared_ptr<ResourceRecord> additionalRecord;
		std::shared_ptr<DnsPacket> packet;
};

TEST_F(DnsPacketTest, TestConstructor)
{
	EXPECT_EQ(1, packet->id());
	EXPECT_EQ(DnsPacket::QueryResponse::RESPONSE, packet->queryResponse());
	EXPECT_EQ(DnsPacket::Opcode::OPCODE_UPDATE, packet->opCode());
	EXPECT_EQ(DnsPacket::AuthoritativeAnswer::AUTHORITATIVE, packet->authoritativeAnswer());
	EXPECT_EQ(DnsPacket::Truncated::NOT_TRUNCATED, packet->truncated());
	EXPECT_EQ(DnsPacket::RecursionDesired::RECURSION_NOT_DESIRED,
	          packet->recursionDesired());
	EXPECT_EQ(DnsPacket::RecursionAvailable::RECURSION_NOT_AVAILABLE,
	          packet->recursionAvailable());
	EXPECT_EQ(DnsPacket::AuthenticatedData::AUTHENTICATED, packet->authenticatedData());
	EXPECT_EQ(DnsPacket::CheckingDisabled::CHECKING_DISABLED, packet->checkingDisabled());
	EXPECT_EQ(DnsPacket::ReturnCode::RETURN_NO_ERROR, packet->returnCode());

	auto questions = packet->questions();
	ASSERT_EQ(1, questions.size());
	EXPECT_EQ(*query, questions.front());

	auto answerRecords = packet->answerRecords();
	ASSERT_EQ(1, answerRecords.size());
	EXPECT_EQ(*answerRecord, answerRecords.front());

	auto authorityRecords = packet->authorityRecords();
	ASSERT_EQ(1, authorityRecords.size());
	EXPECT_EQ(*authorityRecord, authorityRecords.front());

	auto additionalRecords = packet->additionalRecords();
	ASSERT_EQ(1, additionalRecords.size());
	EXPECT_EQ(*additionalRecord, additionalRecords.front());
}

TEST_F(DnsPacketTest, TestToBuffer)
{
	std::vector<std::uint8_t> buffer;
	packet->toBuffer(buffer);

	auto totalDataLength = answerRecord->dataLength() +
	                       authorityRecord->dataLength() +
	                       additionalRecord->dataLength();
	ASSERT_EQ(12 + totalDataLength, buffer.size());

	auto iterator = buffer.cbegin();

	using boost::asio::detail::socket_ops::network_to_host_short;

	// Verify ID
	std::uint16_t id = *iterator++ << 8;
	id |= *iterator++;
	id = network_to_host_short(id);
	EXPECT_EQ(packet->id(), id);/*

	// Verify domain name
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
	EXPECT_EQ(*aRecordData, finalData);*/
}

//TEST(ResourceRecord, TestFromBuffer)
//{
//	auto aRecordData = std::make_shared<ARecordData>("192.168.1.2");
//	ResourceRecord tmpRecord("foo.com", RecordType::A, RecordClass::INTERNET,
//	                         60, aRecordData);

//	std::vector<std::uint8_t> buffer;
//	tmpRecord.toBuffer(buffer);

//	ResourceRecord record(buffer);

//	EXPECT_EQ("foo.com", record.name());
//	EXPECT_EQ(RecordType::A, record.recordType());
//	EXPECT_EQ(RecordClass::INTERNET, record.recordClass());
//	EXPECT_EQ(60, record.timeToLive());

//	ASSERT_EQ(RecordType::A, record.data()->type());
//	EXPECT_EQ(*aRecordData,
//	          *std::static_pointer_cast<ARecordData>(record.data()));
//}
