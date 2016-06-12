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
	ASSERT_EQ(12 + 13 + 22*3 + totalDataLength, buffer.size());

	auto iterator = buffer.cbegin();

	using boost::asio::detail::socket_ops::network_to_host_short;

	// Verify ID
	std::uint16_t id = *iterator++ << 8;
	id |= *iterator++;
	id = network_to_host_short(id);
	EXPECT_EQ(packet->id(), id);

	std::uint8_t byte = *iterator++;

	// Verify query-response
	EXPECT_EQ(packet->queryResponse(), byte >> 7);

	// Verify opcode
	EXPECT_EQ(packet->opCode(), (byte & 0x78) >> 3);

	// Verify authoritate answer
	EXPECT_EQ(packet->authoritativeAnswer(), (byte & 0x04) >> 2);

	// Verify truncation
	EXPECT_EQ(packet->truncated(), (byte & 0x02) >> 1);

	// Verify recusion desired
	EXPECT_EQ(packet->recursionDesired(), byte & 0x01);

	byte = *iterator++;

	// Verify recursion available
	EXPECT_EQ(packet->recursionAvailable(), byte >> 7);

	// Verify authenticated data
	EXPECT_EQ(packet->authenticatedData(), (byte & 0x20) >> 5);

	// Verify checking disabled
	EXPECT_EQ(packet->checkingDisabled(), (byte & 0x10) >> 4);

	// Verify return code
	EXPECT_EQ(packet->returnCode(), byte & 0x0f);

	auto questions = packet->questions();
	auto answerRecords = packet->answerRecords();
	auto authorityRecords = packet->authorityRecords();
	auto additionalRecords = packet->additionalRecords();

	// Verify total questions
	std::uint16_t totalQuestions = *iterator++ << 8;
	totalQuestions |= *iterator++;
	totalQuestions = network_to_host_short(totalQuestions);
	EXPECT_EQ(questions.size(), totalQuestions);

	// Verify total answer records
	std::uint16_t totalAnswerRecords = *iterator++ << 8;
	totalAnswerRecords |= *iterator++;
	totalAnswerRecords = network_to_host_short(totalAnswerRecords);
	EXPECT_EQ(answerRecords.size(), totalAnswerRecords);

	// Verify total authority records
	std::uint16_t totalAuthorityRecords = *iterator++ << 8;
	totalAuthorityRecords |= *iterator++;
	totalAuthorityRecords = network_to_host_short(totalAuthorityRecords);
	EXPECT_EQ(authorityRecords.size(), totalAuthorityRecords);

	// Verify total additional records
	std::uint16_t totalAdditionalRecords = *iterator++ << 8;
	totalAdditionalRecords |= *iterator++;
	totalAdditionalRecords = network_to_host_short(totalAdditionalRecords);
	EXPECT_EQ(additionalRecords.size(), totalAdditionalRecords);

	// Verify query
	Query finalQuery(iterator, buffer.cend());
	EXPECT_EQ(*aRecordData, finalData);

	EXPECT_EQ(*query, questions.front());
	EXPECT_EQ(*answerRecord, answerRecords.front());
	EXPECT_EQ(*authorityRecord, authorityRecords.front());
	EXPECT_EQ(*additionalRecord, additionalRecords.front());
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
